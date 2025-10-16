/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:30:20 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/19 18:46:11 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

t_paths	*allocate_paths(int file_count)
{
	t_paths	*paths;

	paths = (t_paths *)malloc(sizeof(t_paths));
	if (!paths)
		return (NULL);
	paths->count = file_count;
	if (paths->count == 0)
	{
		paths->items = (char **)malloc(sizeof(char *));
		if (!paths->items)
			return (free_paths(paths));
		paths->items[0] = ft_strdup(".");
		if (!paths->items[0])
			return (free_paths(paths));
		paths->count = 1;
		return (paths);
	}
	paths->items = (char **)malloc(sizeof(char *) * paths->count);
	if (!paths->items)
		return (free_paths(paths));
	return (paths);
}

t_paths	*parse_arguments(int argc, char **argv,
	int *file_indices, int file_count)
{
	t_paths	*paths;
	int		i;

	(void) argc;
	i = 0;
	paths = allocate_paths(file_count);
	if (!paths)
		return (NULL);
	while (i < file_count)
	{
		paths->items[i] = ft_strdup(argv[file_indices[i]]);
		if (!paths->items[i])
			return (free_paths(paths));
		i++;
	}
	return (paths);
}

t_paths	*free_paths(t_paths *paths)
{
	int	i;

	if (!paths)
		return (NULL);
	i = 0;
	while (i < paths->count)
		free(paths->items[i++]);
	free(paths->items);
	free(paths);
	return (NULL);
}
