/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 18:56:40 by flplace           #+#    #+#             */
/*   Updated: 2022/03/26 18:57:08 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>

typedef struct t_bld
{
	char	*path[2];
	char	**cmd[2];
	char	**env;
	int		fdin;
	int		fdout;
}	t_bld;

char	**cmdbuilder(char *arg);
char	**ft_split(char const *s, char c);
char	**pathfinder(char **env);
char	*pathbuilder(char **paths, char *cmdname);
char	*pathcrasher(char *path, char *cmdname);
char	*strfinder(char **hay, char *needle);
int		fdsbuilder(char **av, int n);
int		ft_countwords(char const *s, char c);
int		ft_getnxtlen(const char *s, char c);
int		ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
void	*ft_free(char **tab, int words);
void	free_split(char **s);
void	ft_strcat(char *dst, char *src);

#endif
