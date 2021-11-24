/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 16:03:21 by jucapik           #+#    #+#             */
/*   Updated: 2019/05/07 14:39:26 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t	start;
	size_t	end;
	char	*str;

	if (s == NULL)
		return (NULL);
	end = ft_strlen(s);
	start = 0;
	while ((s[start] == ' ' || s[start] == '\n'
				|| s[start] == '\t') && start < end)
		++start;
	if (start == end)
	{
		if ((str = (char *)malloc(sizeof(char))) == NULL)
			return (NULL);
		str[0] = '\0';
		return (str);
	}
	while ((s[end - 1] == ' ' || s[end - 1] == '\n'
				|| s[end - 1] == '\t') && end > 0)
		--end;
	return (ft_strsub(s, start, end - start));
}
