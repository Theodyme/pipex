#include "pipex.h"

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
	while(hay[i])
	{
		j = 0;
		while(hay[i][j] == needle[j])
		{
			j++;
			if(!needle[j])
				return (hay[i]);
		}
		i++;
	}
	return (NULL);
}
