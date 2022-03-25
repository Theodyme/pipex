#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pipex.h"

void	buildfreer(t_bld *s)
{
        close(s->fdin);
        close(s->fdout);
        free(s->cmd[0]);
        free(s->cmd[1]);
        free(s->path[0]);
        free(s->path[1]);
	return ;
}

int	childprocess(t_bld *s, int *pipe)
{
	if (s->fdin == -1)
		return (-1);
	dup2(s->fdin, 0);
	dup2(pipe[1], 1);
	close(s->fdin);
	close(s->fdout);
	close(pipe[0]);
	close(pipe[1]);
	execve(s->path[0], &(s->cmd[0][0]), s->env);
	return (-1);
}

void	parentprocess(t_bld *s, int *pipe)
{
	dup2(pipe[0], 0);
	dup2(s->fdout, 1);
	close(s->fdin);
	close(s->fdout);
	close(pipe[0]);
	close(pipe[1]);
	execve(s->path[1], &(s->cmd[1][0]), s->env);
	return ;
}

void	launcher(t_bld *s)
{
	int	pid;
	int	tuyau[2];
	int	status;

	pid = 1;
	status = 0;
	pipe(tuyau);
	pid = fork();
	if (pid == 0)
	{
		childprocess(s, tuyau);
	} else {
		waitpid(pid, &status, 0);
		parentprocess(s, tuyau);
	}
	return ;
}

int	main(int ac, char **av, char **env)
{
	t_bld	s;

	(void)ac;
	if (!env[0])
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
	s.cmd[0] = cmdbuilder(av[2]);
	s.cmd[1] = cmdbuilder(av[3]);
	s.path[0] = pathbuilder(pathfinder(env), s.cmd[0][0]);
	s.path[1] = pathbuilder(pathfinder(env), s.cmd[1][0]);
	launcher(&s);
	buildfreer(&s);
	return (0);
}

/*

	WHAT WE GLOBALLY NEED TO EXEC AND CAN BE STRUCTURED IN THE EXECUTIVE FUNCTION:
	char	*path;			is the path after going through the pathfinder.c.
	char	**cmd;			is the array containing the command and arguments after going through the cmdbuilder.
	char	**env;			the env variable.
	int	*fds;			is the array containing the INFILE and OUTFILE after going through the fdsbuilder.

	LEAKS HANDLING:
	[ ] you need to free **env/paths= after building your 2nd command.
	[x] you need to free every path you try to build if they fail.
	[x] you need to close any open fd if anything fail and stop the process.
	[ ] you need to free the struct at the potentiel end of the process if anything goes wrong.

	WHICH VAR. WILL BE INSTANTIATED IN THE EXECUTIVE FUNCTION:
	int	pid;			will contain the pid.
	int	*fds[2];		is the array containing the pipes entry & exit.
	int	status;			will contain the status of waitpid(). NEEDS TO CHECK THE MODE AS WELL.

	IF THE TIME LEFT ALLOWS IT, WHAT I MAY NEED IN THE STRUCTURE TO HANDLE ERROR CASES:
	int	errno;

	SPECIFIC CASE TO HANDLE:
	[x]	any command | ls:		only exec ls.
	[x]	echo "hello world" -n:		prints "hello world" -n.
	[x]	grep "o" -c:			prints the result and use the flag.
	[x]					if infile can't be open, need to exec 2nd
	[0]					if outfile can't be open, exec 1st
	[x]					empty cmd

	check leaks with:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes
*/
