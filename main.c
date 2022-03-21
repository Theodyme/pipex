#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef t_info {
	int fdin;
	int fdout;
	int ac;
	int av;
	char **env;
	char **paths;
}	t_info;

/* EXAMPLE WITH

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


int	main(int ac, char **av, char **env)
{
		char *av[] = {"/bin/ls", NULL};
		execve(av[0], av, 0);
	}
	return 0;
}
