/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by amounadi          #+#    #+#             */
/*   Updated: 2024/11/15 22:24:40 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void	init_flags(t_flags *flags)
{
	flags->l = 0;
	flags->r = 0;
	flags->a = 0;
	flags->t = 0;
	flags->cap_r = 0;
}

int	is_flag(char flag)
{
	return (flag == 'l' || flag == 'R' || flag == 'a'
		|| flag == 'r' || flag == 't');
}

void	check_flag(char flag, t_flags *flags)
{
	if (flag == 'l')
		flags->l = 1;
	else if (flag == 'r')
		flags->r = 1;
	else if (flag == 'a')
		flags->a = 1;
	else if (flag == 'R')
		flags->cap_r = 1;
	else if (flag == 't')
		flags->t = 1;
}

int	set_flag(char flag, t_flags *flags)
{
	if (!is_flag(flag))
	{
		ft_printf("ft_ls: illegal option -- %c\n", flag);
		ft_printf("usage: ft_ls [-lRart] [file ...]\n");
		return (0);
	}
	check_flag(flag, flags);
	return (1);
}

int	parse_flags(int argc, char **argv, t_flags *flags, int *file_indices)
{
	int	i;
	int	j;
	int	file_count;

	init_flags(flags);
	i = 1;
	file_count = 0;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			j = 1;
			while (argv[i][j])
				if (!set_flag(argv[i][j++], flags))
					return (-1);
		}
		else
			file_indices[file_count++] = i;
		i++;
	}
	return (file_count);
}
