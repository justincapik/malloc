/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strinsert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:53:05 by jucapik           #+#    #+#             */
/*   Updated: 2019/01/15 12:00:51 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strinsert(char *small, char *large, int pos)
{
	char	*tmp;
	int		size;

	size = ft_strlen(small) + ft_strlen(large);
	tmp = ft_strnew(size);
	ft_strncpy(tmp, large, pos);
	ft_strcat(tmp, small);
	ft_strcat(tmp, large + pos);
	return (tmp);
}
