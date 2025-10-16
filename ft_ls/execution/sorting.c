/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 22:29:34 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/18 22:29:35 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int	compare_files(t_file *a, t_file *b)
{
	return (ft_strcasecmp(a->name, b->name));
}

int	compare_files_reverse(t_file *a, t_file *b)
{
	return (-ft_strcasecmp(a->name, b->name));
}

t_file	*sorted_merge(t_file *a, t_file *b, int (*cmp)(t_file *, t_file *))
{
	t_file	*result;

	result = NULL;
	if (!a)
		return (b);
	if (!b)
		return (a);
	if (cmp(a, b) <= 0)
	{
		result = a;
		result->next = sorted_merge(a->next, b, cmp);
	}
	else
	{
		result = b;
		result->next = sorted_merge(a, b->next, cmp);
	}
	return (result);
}

void	merge_sort(t_file **head_ref, int (*cmp)(t_file *, t_file *))
{
	t_file	*head;
	t_file	*a;
	t_file	*b;

	head = *head_ref;
	if (!head || !head->next)
		return ;
	split_list(head, &a, &b);
	merge_sort(&a, cmp);
	merge_sort(&b, cmp);
	*head_ref = sorted_merge(a, b, cmp);
}

int	compare_by_time(t_file *a, t_file *b)
{
	if (a->stats.st_mtime == b->stats.st_mtime)
	{
		if (a->stats.st_mtim.tv_nsec == b->stats.st_mtim.tv_nsec)
			return (ft_strcasecmp(a->name, b->name));
		return (a->stats.st_mtim.tv_nsec < b->stats.st_mtim.tv_nsec);
	}
	return (a->stats.st_mtime < b->stats.st_mtime);
}
