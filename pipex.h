#ifndef PIPEX_H
# define PIPEX_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "utils.c"
#include "pathfinder.c"
#include "includes/ft_split.c"
#include "includes/ft_strlcpy.c"
#include "includes/ft_strlen.c"

/*
typedef struct t_cmds
{
	int	fdin;
	int	fdout;
}	t_cmds
*/

char	**ft_split(char const *s, char c);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
int	ft_strlen(const char *str);

char	*pathbuilder(char **paths, char *cmdname);
char	*pathcrasher(char *path, char *cmdname);
char	**pathfinder(char **env);
char	*strfinder(char **hay, char *needle);
void	ft_strcat(char *dst, char *src);


# endif
