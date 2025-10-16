/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   long_format.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 22:29:21 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/18 22:29:22 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

char	*get_permissions_helper(mode_t mode, char *perms)
{
	if (S_ISDIR(mode))
		perms[0] = 'd';
	else if (S_ISLNK(mode))
		perms[0] = 'l';
	if (!(mode & S_IRUSR))
		perms[1] = '-';
	if (!(mode & S_IWUSR))
		perms[2] = '-';
	if (!(mode & S_IXUSR))
		perms[3] = '-';
	if (!(mode & S_IRGRP))
		perms[4] = '-';
	if (!(mode & S_IWGRP))
		perms[5] = '-';
	if (!(mode & S_IXGRP))
		perms[6] = '-';
	if (!(mode & S_IROTH))
		perms[7] = '-';
	if (!(mode & S_IWOTH))
		perms[8] = '-';
	if (!(mode & S_IXOTH))
		perms[9] = '-';
	return (perms);
}

char	*get_permissions(mode_t mode)
{
	char	*perms;

	perms = ft_strdup("-rwxrwxrwx");
	return (get_permissions_helper(mode, perms));
}

void	print_long_format_helper(t_file *file)
{
	struct passwd	*pw;
	char			*perms;

	pw = getpwuid(file->stats.st_uid);
	perms = get_permissions(file->stats.st_mode);
	ft_printf("%s %d %s ", perms, file->stats.st_nlink, pw->pw_name);
	free(perms);
}

void	print_long_format(t_file *file)
{
	struct group	*gr;
	char			*time_str;
	char			link_target[4097];
	ssize_t			link_len;

	gr = getgrgid(file->stats.st_gid);
	time_str = get_time(file->stats.st_mtime);
	print_long_format_helper(file);
	ft_printf("%s %ld %s", gr->gr_name, file->stats.st_size, time_str);
	ft_printf(" %s", file->name);
	if (S_ISLNK(file->stats.st_mode))
	{
		link_len = readlink(file->path, link_target, 4097);
		if (link_len != -1)
		{
			link_target[link_len] = '\0';
			ft_printf(" -> %s", link_target);
		}
	}
	ft_printf("\n");
	free(time_str);
}
