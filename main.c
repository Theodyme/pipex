#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pipex.h"

void	childprocess(t_bld *s, int *pipe)
{
	dup2(s->fdin, 0);
	dup2(pipe[1], 1);
	close(s->fdin);
	close(s->fdout);
	close(pipe[0]);
	close(pipe[1]);
	execve(s->path[0], &(s->cmd[0][0]), s->env);
	return ;
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
}

int	main(int ac, char **av, char **env)
{
	t_bld	s;

	if (!env[0])
		return (0);
	s.env = env;
	s.fdin = fdsbuilder(av, 1);
	s.fdout = fdsbuilder(av, 2);
	s.cmd[0] = cmdbuilder(av[2]);
	s.cmd[1] = cmdbuilder(av[3]);
	s.path[0] = pathbuilder(pathfinder(env), s.cmd[0][0]);
	s.path[1] = pathbuilder(pathfinder(env), s.cmd[1][0]);
/*	CHECKER DANS UN WHILE SUR LA STRUCTURE SI IL Y A UN NULL OU -1 ANYWHERE POUR RETURN UNE ERREUR;
	if(s.cmd[1][0] == "ls")
	{
		dup2(s.fdout, 1);
		close(s.fdin, s.fdout);
		execve(s.path[1], s.cmd[1], env);
		return (0);
	}
*/	launcher(&s);
	close(s.fdin);
	close(s.fdout);
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
	[ ] you need to close any open fd if anything fail and stop the process.
	[ ] you need to free the struct at the potentiel end of the process if anything goes wrong.

	WHICH VAR. WILL BE INSTANTIATED IN THE EXECUTIVE FUNCTION:
	int	pid;			will contain the pid.
	int	*fds[2];		is the array containing the pipes entry & exit.
	int	status;			will contain the status of waitpid(). NEEDS TO CHECK THE MODE AS WELL.

	IF THE TIME LEFT ALLOWS IT, WHAT I MAY NEED IN THE STRUCTURE TO HANDLE ERROR CASES:
	int	errno;

	SPECIFIC CASE TO HANDLE:
	any command | ls:		only exec ls.
	echo "hello world" -n:		prints "hello world" -n.
	grep "o" -c:			prints the result and use the flag.




	int	pid;
	int	i = 0;
	int	fdin;
	int	fdout;
	int	fds[2];
	int	status;

	char **path;
	char *bleh;
	char *blah;
	char **cmd1;
	char **cmd2;
	cmd1 = cmdbuilder(av[2]);
	cmd2 = cmdbuilder(av[3]);
	path = pathfinder(env);
	bleh = pathbuilder(path, cmd1[0]);
	blah = pathbuilder(path, cmd2[0]);

	fdin = fdsbuilder(av, 1);
	fdout = fdsbuilder(av, 2);

	printf("fdin: %d, fdout: %d\n", fdin, fdout);
	printf("LES PATHS:\n%s\n%s\n", bleh, blah);
	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fdin, 0);
		close(fdin);
		close(fdout);
		dup2(fds[1], 1);
		close(fds[1]);
		execve(bleh, cmd1, env);
	} else {
		waitpid(pid, &status, 0);
		close(fdin);
		close(fds[1]);
		dup2(fds[0], 0);
		dup2(fdout, 1);
		close(fdout);
		close(fds[0]);
		execve(blah, cmd2, env);
	}
	return (1);
}
*/
