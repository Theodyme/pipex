/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flplace <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 20:00:23 by flplace           #+#    #+#             */
/*   Updated: 2021/02/26 16:13:37 by flplace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_countwords(char const *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

int	ft_getnxtlen(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
	{
		i++;
	}
	return (i);
}

void	*ft_free(char **tab, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free(tab[i++]);
	}
	free(tab);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		i;
	int		words;

	i = 0;
	if (!s)
		return (NULL);
	words = ft_countwords(s, c);
	tab = malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	while (i < words && (*s != 0 && *s == c))
		s++;
	while (i < words)
	{
		tab[i] = malloc(sizeof(char) * (ft_getnxtlen(s, c) + 1));
		if (!tab[i])
			return (ft_free(tab, words));
		ft_strlcpy(tab[i++], s, ft_getnxtlen(s, c) + 1);
		s += ft_getnxtlen(s, c);
		while (*s != 0 && *s == c)
			s++;
	}
	tab[i] = 0;
	return (tab);
}
