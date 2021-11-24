/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 11:32:56 by jucapik           #+#    #+#             */
/*   Updated: 2018/11/15 16:59:47 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(f)(t_list *elem))
{
	t_list	*nouv;
	t_list	*tmp;
	t_list	*start;

	if (lst == NULL)
		return (NULL);
	nouv = f(lst);
	if ((nouv = ft_lstnew(nouv->content, nouv->content_size)) == NULL)
		return (NULL);
	tmp = nouv;
	tmp->next = nouv;
	start = nouv;
	lst = lst->next;
	while (lst != NULL)
	{
		nouv = f(lst);
		if ((nouv = ft_lstnew(nouv->content, nouv->content_size)) == NULL)
			return (NULL);
		tmp->next = nouv;
		tmp = nouv;
		lst = lst->next;
	}
	return (start);
}
