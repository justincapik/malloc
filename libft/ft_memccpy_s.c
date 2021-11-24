/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy_s.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 13:11:37 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/15 15:44:30 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy_s(void *dst, const void *src, int c, size_t n)
{
	size_t			i;
	unsigned char	a;
	unsigned char	*d;
	unsigned char	*s;

	i = 0;
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	a = (unsigned char)c;
	if (dst == NULL || src == NULL || n == 0)
		return (NULL);
	while (i < n && s[i] != a)
	{
		d[i] = s[i];
		i++;
	}
	if (s[i] == a)
	{
		d[i] = a;
		return (&d[i + 1]);
	}
	return (NULL);
}
