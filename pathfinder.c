#include "pipex.h"

char	*pathcrasher(char *path, char *cmdname)
{
	char	*to_access;

	to_access = (char *)calloc(sizeof(char), ft_strlen(path) + ft_strlen(cmdname) + 2);
	ft_strcat(to_access, path);
	ft_strcat(to_access, "/");
	ft_strcat(to_access, cmdname);
	return (to_access);
}

char	*pathbuilder(char **paths, char *cmdname)
{
	char	*path;
	int	i;

	i = 0;
	path = NULL;
	if (access(cmdname, F_OK) == 0)
		path = cmdname;
	while (paths && paths[++i] && path == NULL)
	{
		path = pathcrasher(paths[i], cmdname);
		if(access(path, F_OK) == 0)
			break ;
		free(path);
		path = NULL;
	}
	return (path);
}

char	**pathfinder(char **env)
{
	char	**paths;
	char	*line;

	line = strfinder(env, "PATH=");
	if (line != NULL)
	{
		paths = ft_split((line + 5), ':');
		return (paths);
	}
	return (NULL);
}
