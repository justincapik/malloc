/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 13:00:31 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/12 16:57:37 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char	*ft_strncpy(char *dest, const char *src, size_t len)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	s = (unsigned char *)src;
	d = (unsigned char *)dest;
	i = 0;
	while (s[i] != '\0' && i < len)
	{
		d[i] = s[i];
		i++;
	}
	if (s[i] == '\0')
	{
		while (i < len)
			d[i++] = '\0';
	}
	return (dest);
}
