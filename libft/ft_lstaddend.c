/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstaddend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 15:46:18 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/15 16:05:07 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_lstaddend(t_list *alst, t_list *new)
{
	t_list *tmp;

	if (alst == NULL)
		return (0);
	tmp = alst;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}
