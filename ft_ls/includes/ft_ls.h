/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/18 17:11:26 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "../libft/libft.h"
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <sys/types.h>
# include <errno.h>
# include <string.h>
# include <sys/ioctl.h>
# include <locale.h>
# include <sys/stat.h>
# include <limits.h>

typedef struct s_flags
{
	int	l;
	int	r;
	int	a;
	int	t;
	int	cap_r;

}	t_flags;

typedef struct s_file
{
	char			*name;
	char			*path;
	struct stat		stats;
	struct s_file	*next;
}	t_file;

typedef struct s_paths
{
	char			**items;
	int				count;
}	t_paths;

/* Parsing functions */
int		parse_flags(int argc, char **argv, t_flags *f, int *file_indices);
t_paths	*parse_arguments(int argc, char **argv, int *f, int fa);
t_paths	*free_paths(t_paths *paths);
t_file	*parse_path(char *path);

/*execution function */
void	exec_ls(t_paths paths, t_flags flags);
void	handle_file(char *path, t_flags *flags);
void	list_directory(char *path, t_flags *flags);
void	print_files(t_file *files);
void	handle_cap_R(char *path, t_flags *flags);

/* Long format helper functions */
void	handle_long_format(t_file *files);
char	*get_permissions(mode_t mode);
char	*get_time(time_t mtime);
void	print_long_format(t_file *file);
long	int	get_total_blocks(t_file *files);
t_file	*get_subdirs(char *path, t_flags *flags);

/* Sorting functions */
t_file	*sorted_merge(t_file *a, t_file *b, int (*cmp)(t_file *, t_file *));
int		compare_files_reverse(t_file *a, t_file *b);
int		compare_files(t_file *a, t_file *b);
void	merge_sort(t_file **head_ref, int (*cmp)(t_file *, t_file *));
int		compare_by_time(t_file *a, t_file *b);

/* Utility functions */
char	*safe_path_join(const char *path1, const char *path2);
void	split_list(t_file *source, t_file **front_ref, t_file **back_ref);
void	free_dir_list(t_file *dirs);
void	sort_subdirs(t_file **subdirs, t_flags *flags);

#endif
