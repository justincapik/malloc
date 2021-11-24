/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 16:40:24 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/11 15:07:49 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	getnblen(int n)
{
	int i;

	i = 1;
	if (n < 0)
		i++;
	if (-10 < n && n < 10)
		return (i);
	while (-10 >= n || n >= 10)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char		*ft_itoa(int n)
{
	int		size;
	char	*res;
	int		i;
	int		signe;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	size = getnblen(n);
	res = (char *)malloc(sizeof(char) * (size + 1));
	if (res == NULL)
		return (NULL);
	i = size - 1;
	signe = (n < 0) ? -1 : 1;
	if (n < 0)
		n = -n;
	while (i >= 0)
	{
		res[i--] = n % 10 + '0';
		n /= 10;
	}
	if (signe == -1)
		res[0] = '-';
	res[size] = '\0';
	return (res);
}
