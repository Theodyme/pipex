#include <stdio.h>
#include "pipex.h"
#include "includes/library.h"

int	main(int ac, char **av, char **env)
{
	int	pid;
	int	i = 0;
	int	fds[2];
	int	status;

	char **path;
	char *bleh;
	path = pathfinder(env);
	bleh = pathbuilder(path, av[1]);

	printf("%s\n", bleh);
//	pipe(fds);
//	pid = fork();
//	if (pid == 0)
//	{
//		close(fds[0]);
//		dup2(fds[1], 1);
//		close(fds[1]);
		execv(bleh, &av[1]);
//	} else {
//		close(fds[1]);
///		dup2(fds[0], 0);
//		close(fds[0]);
//		execv(bleh, av);
//	}
//	printf("%s\n", bleh);
	return (1);
}
/*
typedef t_info {
	int fdin;
	int fdout;
	int ac;
	int av;
	char **env;
	char **paths;
}	t_info;

EXAMPLE WITH

> ./pipex infile "grep o" "wc -w" outfile

main- set structures info and send it to the launcher

launcher- fork happens here.
launch ex1 and ex2 as long as there're commands to execute and handle the pipes.
returns the error signal if failed or a success signal if everything went smoothly.
needs: int fdin, int fdout, int ac

p1- a function to find 'PATH' in env and extract line in **paths
needs: char **env
returns: char **paths

p2- a function to test (access) every path
needs: char **paths, cmd_name
returns: char *path

p3- a function to rebuild the command with flags (needs (b) and ft_split)
needs: char *cmd
returns: char **args

ex1- execve if the process is a child
needs: char **env, char **args
returns: 0 or error signal

ex2- process the parent
needs:

/!\ ajouter la gestion d'erreur !!
*/
