/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset_s.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 09:52:43 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/15 15:45:26 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset_s(void *b, int c, size_t len)
{
	unsigned char	*p;
	unsigned char	a;

	if (b == NULL)
		return (NULL);
	p = (unsigned char *)b;
	a = (unsigned char)c;
	if (len == 0)
		return (b);
	while (len > 0)
	{
		*p = a;
		len--;
		p++;
	}
	return (b);
}
