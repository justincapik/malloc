/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 12:37:55 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/13 16:22:45 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;
	size_t			i;
	unsigned char	a;

	i = 0;
	p = (unsigned char *)s;
	a = (unsigned char)c;
	while (i < n)
	{
		if (p[i] == a)
			return (&p[i]);
		i++;
	}
	return (NULL);
}
