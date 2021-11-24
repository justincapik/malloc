/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 12:14:16 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/12 13:02:12 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void			*tab;
	unsigned char	*p;
	size_t			i;

	if ((tab = malloc(size)) == NULL)
		return (NULL);
	i = 0;
	p = tab;
	while (i < size)
		p[i++] = '\0';
	return (tab);
}
