#include "pipex.h"

char	**cmdbuilder(char *arg)
{
	char **cmds;

	
	cmds = ft_split(arg, ' ');
	return (cmds);
}

int	fdsbuilder(char **av, int n)
{
	int	fd;

	if (n == 1)
		fd = open(av[1], O_RDONLY, 0777);
	if (n == 2)
		fd = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	return (fd);
}
