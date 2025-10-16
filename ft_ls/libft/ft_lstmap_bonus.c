/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <mounadi1337@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 10:43:10 by amounadi          #+#    #+#             */
/*   Updated: 2021/11/08 18:09:06 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*tmp;
	t_list	*head;

	if (!lst)
		return (NULL);
	tmp = ft_lstnew(f(lst->content));
	head = tmp;
	while (lst->next)
	{
		if (!tmp)
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		lst = lst->next;
		tmp->next = ft_lstnew(f(lst->content));
		tmp = tmp->next;
	}
	return (head);
}
