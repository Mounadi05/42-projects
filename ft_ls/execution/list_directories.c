/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_directories.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 22:29:14 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/19 18:54:38 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

static t_file	*process_directory_entry(char *path, struct dirent *entry,
				t_flags *flags)
{
	t_file		*tmp;
	struct stat	stats;
	char		*full_path;

	if (entry->d_name[0] == '.' && !flags->a)
		return (NULL);
	full_path = safe_path_join(path, entry->d_name);
	if (!full_path || lstat(full_path, &stats) == -1)
	{
		if (full_path)
		{
			ft_printf("ft_ls: %s: %s\n", full_path, strerror(errno));
			free(full_path);
		}
		return (NULL);
	}
	tmp = (t_file *)malloc(sizeof(t_file));
	tmp->name = ft_strdup(entry->d_name);
	tmp->path = full_path;
	tmp->stats = stats;
	return (tmp);
}

static void	apply_sort(t_file **files, t_flags *flags)
{
	if (flags->t && flags->r)
		merge_sort(files, compare_files_reverse);
	else if (flags->t)
		merge_sort(files, compare_by_time);
	else if (flags->r)
		merge_sort(files, compare_files_reverse);
	else
		merge_sort(files, compare_files);
}

static void	sort_and_print_files(t_file *files, t_flags *flags)
{
	t_file	*tmp;

	tmp = files;
	apply_sort(&tmp, flags);
	if (flags->l)
		handle_long_format(tmp);
	else
		print_files(tmp);
	free_dir_list(tmp);
}

static void	helper(t_file *files, t_flags *flags, DIR *dir)
{
	closedir(dir);
	sort_and_print_files(files, flags);
}

void	list_directory(char *path, t_flags *flags)
{
	t_file			*files;
	t_file			*tmp;
	DIR				*dir;
	struct dirent	*entry;

	files = NULL;
	dir = opendir(path);
	if (!dir)
	{
		ft_printf("ft_ls: %s: %s\n", path, strerror(errno));
		return ;
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		tmp = process_directory_entry(path, entry, flags);
		if (tmp)
		{
			tmp->next = files;
			files = tmp;
		}
		entry = readdir(dir);
	}
	helper(files, flags, dir);
}
