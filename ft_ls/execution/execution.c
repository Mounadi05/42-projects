/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 22:29:38 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/19 18:53:19 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void	handle_recursive(char *path, t_flags *flags)
{
	struct stat	stats;
	t_file		*subdirs;
	t_file		*tmp;

	if (lstat(path, &stats) == -1)
	{
		ft_printf("ft_ls: %s: %s\n", path, strerror(errno));
		return ;
	}
	list_directory(path, flags);
	ft_printf("\n");
	subdirs = get_subdirs(path, flags);
	if (!subdirs)
		return ;
	tmp = subdirs;
	sort_subdirs(&subdirs, flags);
	while (tmp)
	{
		ft_printf("\n%s:\n", tmp->path);
		handle_recursive(tmp->path, flags);
		tmp = tmp->next;
	}
	free_dir_list(subdirs);
}

void	free_dir_list(t_file *dirs)
{
	t_file	*tmp;

	while (dirs)
	{
		tmp = dirs;
		dirs = dirs->next;
		free(tmp->name);
		free(tmp->path);
		free(tmp);
	}
}

void	sort_subdirs(t_file **subdirs, t_flags *flags)
{
	if (flags->r)
		merge_sort(subdirs, compare_files_reverse);
	else if (flags->t)
		merge_sort(subdirs, compare_by_time);
	else
		merge_sort(subdirs, compare_files);
}

void	exec_ls(t_paths paths, t_flags flags)
{
	struct stat	stats;
	int			i;

	i = 0;
	while (i < paths.count)
	{
		if (lstat(paths.items[i], &stats) == -1)
		{
			ft_printf("ft_ls: %s: %s\n", paths.items[i], strerror(errno));
		}
		else if (S_ISDIR(stats.st_mode))
		{
			if (flags.cap_r)
				handle_recursive(paths.items[i], &flags);
			else
				list_directory(paths.items[i], &flags);
		}
		else
			handle_file(paths.items[i], &flags);
		i++;
	}
}
