/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 22:29:31 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/19 18:28:30 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

char	*safe_path_join(const char *path1, const char *path2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = ft_strlen(path1);
	len2 = ft_strlen(path2);
	result = malloc(len1 + len2 + 2);
	if (!result)
		return (NULL);
	ft_strlcpy(result, path1, len1 + 1);
	if (len1 > 0 && path1[len1 - 1] != '/')
		result[len1++] = '/';
	ft_strlcpy(result + len1, path2, len2 + 1);
	return (result);
}

void	split_list(t_file *source, t_file **front_ref, t_file **back_ref)
{
	t_file	*f;
	t_file	*s;

	s = source;
	f = source->next;
	while (f)
	{
		f = f->next;
		if (f)
		{
			s = s->next;
			f = f->next;
		}
	}
	*front_ref = source;
	*back_ref = s->next;
	s->next = NULL;
}

t_file	*create_dir_node(char *path, char *name, struct stat stats)
{
	t_file	*new_dir;
	char	*full_path;

	full_path = safe_path_join(path, name);
	if (!full_path)
		return (NULL);
	new_dir = (t_file *)malloc(sizeof(t_file));
	if (!new_dir)
	{
		free(full_path);
		return (NULL);
	}
	new_dir->name = ft_strdup(name);
	new_dir->path = full_path;
	new_dir->stats = stats;
	new_dir->next = NULL;
	return (new_dir);
}

static t_file	*process_dir_entry(char *path, struct dirent *entry,
		t_flags *flags)
{
	struct stat	stats;
	t_file		*new_dir;
	char		*full_path;

	new_dir = NULL;
	if (entry->d_name[0] == '.' && (!flags->a
			|| ft_strcmp(entry->d_name, ".") == 0
			|| ft_strcmp(entry->d_name, "..") == 0))
		return (NULL);
	full_path = safe_path_join(path, entry->d_name);
	if (!full_path)
		return (NULL);
	if (lstat(full_path, &stats) != -1 && S_ISDIR(stats.st_mode))
		new_dir = create_dir_node(path, entry->d_name, stats);
	free(full_path);
	return (new_dir);
}

t_file	*get_subdirs(char *path, t_flags *flags)
{
	DIR				*dir;
	struct dirent	*entry;
	t_file			*dirs;
	t_file			*new_dir;

	dir = opendir(path);
	if (!dir)
		return (NULL);
	dirs = NULL;
	entry = readdir(dir);
	while (entry != NULL)
	{
		new_dir = process_dir_entry(path, entry, flags);
		if (new_dir)
		{
			new_dir->next = dirs;
			dirs = new_dir;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (dirs);
}
