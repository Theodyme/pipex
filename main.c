/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 18:56:35 by flplace           #+#    #+#             */
/*   Updated: 2022/03/27 16:37:09 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	WHAT WE GLOBALLY NEED TO EXEC AND CAN BE STRUCTURED IN THE MAIN FUNCTION:
	char	*path;			is the path after going through the pathfinder.c.
	char	**cmd;			is the array containing the command and arguments after going through the cmdbuilder.
	char	**env;			the env variable.
	int	*fds;			is the array containing the INFILE and OUTFILE fds after going through the fdsbuilder.

	if you check that those parameters are valid before properly starting anything, you will barely have to worry about fd closing
	and variables that must be freed.

	LEAKS HANDLING:
	[x] free **env/paths= after building your 2nd command.
	[x] free every path you try to build if they fail.
	[x] be sure to close any unused fd before launching execve.
	[x] close any open fd if anything fails and stops the process.
	[x] you need to free the struct's elements at the potential end of the process if anything goes wrong.

	WHICH VAR. WILL BE INSTANTIATED IN THE EXECUTIVE FUNCTION:
	int	pid;			will contain the pid.
	int	*fds[2];		is the array containing the pipes entry & exit.
	int	status;			will contain the status of waitpid().

	IF THE TIME LEFT ALLOWS IT, WHAT I MAY NEED IN THE STRUCTURE TO HANDLE ERROR CASES:
	int	errno;

	SPECIFIC CASE TO HANDLE:
	[x]	any command | ls:		only executes ls.
	[0]	echo "hello world" -n:		prints "hello world" -n. (YOU DON'T HAVE TO HANDLE THIS CASE TO SUCCESS)
	[0]	grep "o" -c:			prints the result and use the flag. (YOU DON'T HAVE TO HANDLE THIS CASE TO SUCCESS)
						WHY ? First, because Pipex is not about parsing. As long as you understand how pipes,
						fork and fd manipulation work, you should be good. You can also argue that your commands
						are typed inside quotes, and that those cases will mess with how many arguments your
						program will process.
	[x]					if infile can't be open (doesn't exist or you don't have the right permission), you need
						to execute 2nd command nonetheless.
	[x]					if outfile can't be open (wrong permission), you need to execute 1st command nonetheless.
						It won't necessarily shows in the outfile, but it can be checked with commands like ls.
	[x]					handle empty cmd.
	[x]					you need to check if the **env parameter exists.
	[x]					you need to check if you can actually find the line PATH= in the env parameter.
						This will ensure your program won't segfault if you unset PATH in the shell before
						running Pipex.
	[x]					handle wrong first cmd.
	[x]					handle wrong second cmd.

	check leaks on linux with:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --tracks-fds=yes ./pipex -
*/


//	BUILDFREER: will free the struct's elements.

void	buildfreer(t_bld *s)
{
	if (s->fdin != -1)
		close(s->fdin);
	if (s->fdout != -1)
		close(s->fdout);
	free_split(s->cmd[0]);
	free_split(s->cmd[1]);
	free(s->path[0]);
	free(s->path[1]);
	return ;
}

//	CHILDPROCESS: will handle the fds, pipe, and then execute the first command.

int	childprocess(t_bld *s, int *pipe)
{
	if (s->fdin == -1 || !(s->path[0]))
	{
		buildfreer(s);
		close(pipe[0]);
		close(pipe[1]);
		exit(1);
	}
	dup2(s->fdin, 0);
	dup2(pipe[1], 1);
	close(pipe[0]);
	close(pipe[1]);
	close(s->fdin);
	close(s->fdout);
	execve(s->path[0], &(s->cmd[0][0]), s->env);
	return (-1);
}

//	PARENTPROCESS: [should be renamed] will handle the fds, pipe, and then execute the second command.

void	parentprocess(t_bld *s, int *pipe)
{
	if (!(s->path[1]))
	{
		close(pipe[1]);
		close(pipe[0]);
		return ;
	}
	dup2(pipe[0], 0);
	dup2(s->fdout, 1);
	close(s->fdin);
	close(s->fdout);
	close(pipe[0]);
	close(pipe[1]);
	execve(s->path[1], &(s->cmd[1][0]), s->env);
	return ;
}

//	LAUNCHER: will fork your program to execute each command in their own children. will pipe before that to ensure you have an
//	accessible pipe to use to pass information between process. Waitpid() is executed at the end of the two forks to ensure the
//	handling of commands like sleep, for which it is important that the two commands start at the same time.

void	launcher(t_bld *s)
{
	int	pid[2];
	int	tuyau[2];
	int	status;

	status = 0;
	pipe(tuyau);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		childprocess(s, tuyau);
	}
	else
	{
		pid[1] = fork();
		if (pid[1] == 0)
			parentprocess(s, tuyau);
		else
		{
			close(tuyau[0]);
			close(tuyau[1]);
			waitpid(pid[0], &status, 0);
			waitpid(pid[1], &status, 0);
		}
	}
	return ;
}

//	MAIN: in which you will fill your structure with any necessary parameters to execute.
//	You will need to get into pathbuilders functions to get the path string out of the **env parameter.
//	I'm building the command and flags array to immediately check its validity.
//	Only then do I launch pipex properly-speaking.

int	main(int ac, char **av, char **env)
{
	t_bld	s;

	if (ac != 5 || !env[0])
		return (0);
	s.env = env;
	s.fdin = fdsbuilder(av, 1);
	s.fdout = fdsbuilder(av, 2);
	if (!av[2][0] || !av[3][0] || (!av[2][0] && !av[3][0]))
	{
		write(0, "error: command not found.", 25);
		close(s.fdin);
		close(s.fdout);
		return (0);
	}
	if (s.fdout >= 0)
	{
		s.cmd[0] = cmdbuilder(av[2]);
		s.cmd[1] = cmdbuilder(av[3]);
		s.path[0] = pathbuilder(pathfinder(env), s.cmd[0][0]);
		s.path[1] = pathbuilder(pathfinder(env), s.cmd[1][0]);
		launcher(&s);
		buildfreer(&s);
	}
	close(s.fdin);
	return (0);
}
