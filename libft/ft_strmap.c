/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 13:39:54 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/14 14:01:52 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
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
		ret[i] = f(s[i]);
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
