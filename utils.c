/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <flplace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 19:00:59 by flplace           #+#    #+#             */
/*   Updated: 2022/03/26 19:01:31 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

int	ft_strlen(const char *str)
{
	char	*i;

	i = (char *)str;
	while (*i)
		i++;
	return (i - str);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!src)
		return (0);
	if (dstsize != 0)
	{
		while (src[i] && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
	}
	if (i < dstsize)
		dst[i] = '\0';
	return (ft_strlen((char *)src));
}

void	ft_strcat(char *dst, char *src)
{
	while (*dst)
		dst++;
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
	return ;
}

char	*strfinder(char **hay, char *needle)
{
	int	i;
	int	j;

	i = 0;
	while (hay[i])
	{
		j = 0;
		while (hay[i][j] == needle[j])
		{
			j++;
			if (!needle[j])
				return (hay[i]);
		}
		i++;
	}
	return (NULL);
}

void	free_split(char **s)
{
	char	**tmp;

	tmp = s;
	while (*s)
	{
		free(*s);
		s++;
	}
	free(tmp);
}
