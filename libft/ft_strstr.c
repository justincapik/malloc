/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 17:24:28 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/14 14:00:12 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *hs, const char *needle)
{
	int		i;
	int		j;

	if (*needle == '\0')
		return ((char *)hs);
	if (*hs == '\0')
		return (NULL);
	j = 0;
	while (hs[j] != '\0')
	{
		if (hs[j] == needle[0])
		{
			i = 0;
			while (hs[i + j] && needle[i] && hs[i + j] == needle[i])
				i++;
			if (needle[i] == '\0')
				return ((char *)&(hs[j]));
		}
		j++;
	}
	return (NULL);
}
