/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 13:09:38 by jucapik           #+#    #+#             */
/*   Updated: 2018/12/22 13:23:30 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_atoi(char *str)
{
	int nb;
	int negpos;
	int j;
	int i;

	negpos = 1;
	nb = 0;
	j = 0;
	while (str[j] == ' ' || str[j] == '\n' || str[j] == '\t' || str[j] == '\v'
			|| str[j] == '\r' || str[j] == '\b' || str[j] == '\f')
		++j;
	if (str[j] == '-' || str[j] == '+')
		negpos = (str[j++] == '-') ? -1 : 1;
	i = 0;
	while (str[j] >= '0' && str[j] <= '9')
	{
		nb = nb * 10 + str[j++] - '0';
		++i;
	}
	if (i >= 20)
		return ((negpos > 0) ? -1 : 0);
	nb *= negpos;
	return (nb);
}
