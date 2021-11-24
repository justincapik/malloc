/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strequ.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 15:01:14 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/15 13:00:01 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strnequ(char const *s1, char const *s2, size_t n)
{
	size_t i;

	if ((s1 == NULL && s2 == NULL) || n == 0)
		return (1);
	if (s1 == NULL || s2 == NULL)
		return (0);
	if ((*s1 == '\0' && *s2 == '\0'))
		return (1);
	if (*s1 == '\0' || *s2 == '\0')
		return (0);
	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && i < n)
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	if (i == n && s1[i - 1] == s2[i - 1])
		return (1);
	if ((i == n && s1[i - 1] != s2[i - 1]))
		return (0);
	if (s1[i] == s2[i])
		return (1);
	return (0);
}
