/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:30:00 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/19 16:40:20 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_ls.h"

int	main(int argc, char **argv)
{
	t_flags	flags;
	int		*file_indices;
	int		file_count;
	t_paths	*paths;

	file_indices = (int *)malloc(sizeof(int) * argc);
	if (!file_indices)
		return (1);
	file_count = parse_flags(argc, argv, &flags, file_indices);
	if (file_count == -1)
	{
		free(file_indices);
		return (1);
	}
	paths = parse_arguments(argc, argv, file_indices, file_count);
	free(file_indices);
	if (!paths)
		return (1);
	exec_ls(*paths, flags);
	free_paths(paths);
	return (0);
}
