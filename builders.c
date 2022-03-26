/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builders.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 18:56:38 by flplace           #+#    #+#             */
/*   Updated: 2022/03/26 18:57:08 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**cmdbuilder(char *arg)
{
	char	**cmds;

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
	if (fd == -1)
		write(0, "error: can't access file.", 26);
	return (fd);
}
