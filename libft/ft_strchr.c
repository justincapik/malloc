/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 17:04:15 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/09 16:50:05 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char *ret;

	if (c == 0)
		return ((char *)&s[ft_strlen(s)]);
	while (*s)
	{
		if (*s == c)
		{
			ret = (char *)s;
			return (ret);
		}
		s++;
	}
	return (NULL);
}
