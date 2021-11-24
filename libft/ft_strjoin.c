/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 15:45:37 by jucapik           #+#    #+#             */
/*   Updated: 2018/12/20 11:16:12 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	i;
	size_t	lims1;
	size_t	lims2;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	lims1 = ft_strlen(s1);
	lims2 = ft_strlen(s2);
	if ((res = (char *)malloc(sizeof(char) * (lims1 + lims2 + 1))) == NULL)
		return (NULL);
	i = 0;
	while (i < lims1)
	{
		res[i] = s1[i];
		i++;
	}
	while (i < lims1 + lims2)
	{
		res[i] = s2[i - lims1];
		i++;
	}
	res[i] = '\0';
	return (res);
}
