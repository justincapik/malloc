/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 14:51:04 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/14 14:02:31 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*ret;
	size_t	i;
	size_t	lim;

	if (s == NULL)
		return (NULL);
	lim = ft_strlen(s);
	if ((ret = (char *)malloc(sizeof(char) * (lim + 1))) == NULL)
		return (NULL);
	i = 0;
	while (i < lim)
	{
		ret[i] = f(i, s[i]);
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
