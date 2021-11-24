/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 15:06:28 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/15 11:47:59 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t i;
	size_t sl;
	size_t sd;

	sl = ft_strlen(src);
	sd = ft_strlen(dst);
	i = 0;
	if (size <= sd)
		return (sl + size);
	while (src[i] != '\0' && i < size - sd - 1)
	{
		dst[sd + i] = src[i];
		i++;
	}
	if (i < size - sd)
		dst[sd + i] = '\0';
	return (sd + sl);
}
