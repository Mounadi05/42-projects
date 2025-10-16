/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   long_format_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 22:29:25 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/19 18:43:10 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void	handle_long_format(t_file *files)
{
	t_file	*tmp;

	tmp = files;
	ft_printf("total %ld\n", get_total_blocks(files) / 2);
	while (tmp)
	{
		print_long_format(tmp);
		tmp = tmp->next;
	}
}

long int	get_total_blocks(t_file *files)
{
	long int	total;
	t_file		*tmp;

	total = 0;
	tmp = files;
	while (tmp)
	{
		total += tmp->stats.st_blocks;
		tmp = tmp->next;
	}
	return (total);
}

char	*get_time(time_t mtime)
{
	char	*time_str;
	char	*form;

	time_str = ctime(&mtime);
	form = malloc(13);
	ft_memcpy(form, time_str + 4, 12);
	form[12] = '\0';
	return (form);
}

void	handle_file(char *path, t_flags *flags)
{
	struct stat	stats;
	t_file		*file;

	if (lstat(path, &stats) == -1)
	{
		ft_printf("ft_ls: %s: %s\n", path, strerror(errno));
		return ;
	}
	file = (t_file *)malloc(sizeof(t_file));
	file->name = ft_strdup(path);
	file->path = ft_strdup(path);
	file->stats = stats;
	file->next = NULL;
	if (flags->l)
		print_long_format(file);
	else
		ft_printf("%s\n", file->name);
	free_dir_list(file);
}

void	print_files(t_file *files)
{
	t_file	*tmp;

	tmp = files;
	while (tmp)
	{
		if (tmp->next)
			ft_printf("%s  ", tmp->name);
		else
			ft_printf("%s\n", tmp->name);
		tmp = tmp->next;
	}
}
