/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 16:16:43 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/15 16:56:24 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	getnbstr(char const *s, char c)
{
	size_t	i;
	size_t	nb;

	nb = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] == '\0')
		{
			if (nb == 0)
				return (1);
			else
				return (nb);
		}
		while (s[i] != c && s[i] != '\0')
			i++;
		nb++;
	}
	return (nb);
}

static char		*getstr(char const *s, char c)
{
	size_t	size;
	size_t	i;
	char	*mot;

	size = 0;
	while (s[size] != '\0' && s[size] != c)
		size++;
	if ((mot = (char *)malloc(sizeof(char) * (size + 1))) == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		mot[i] = s[i];
		i++;
	}
	mot[i] = '\0';
	return (mot);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**res;
	size_t	limwrd;
	size_t	i;

	if (s == NULL)
		return (NULL);
	limwrd = getnbstr(s, c);
	if ((res = (char **)malloc(sizeof(char *) * (limwrd + 1))) == NULL)
		return (NULL);
	i = 0;
	while (*s != '\0' && *s == c)
		s++;
	while (*s != '\0' && i < limwrd)
	{
		res[i++] = getstr(s, c);
		while (*s != '\0' && *s != c)
			s++;
		while (*s != '\0' && *s == c)
			s++;
	}
	res[i] = NULL;
	return (res);
}
