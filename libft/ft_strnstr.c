/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 17:39:12 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/12 17:02:12 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

char	*ft_strnstr(const char *hs, const char *needle, size_t len)
{
	size_t		j;
	size_t		i;

	ft_strlen(hs);
	if (*needle == '\0')
		return ((char *)hs);
	if (*hs == '\0')
		return (NULL);
	if (len == 0)
		return (NULL);
	j = 0;
	while (hs[j] != '\0' && j < len)
	{
		i = 0;
		while (hs[j + i] == needle[i] && needle[i] && hs[j + i]
				&& i + j < len)
			i++;
		if (needle[i] == '\0')
			return ((char *)&(hs[j]));
		j++;
	}
	return (NULL);
}
