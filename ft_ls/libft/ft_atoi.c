/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <mounadi1337@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 11:51:28 by amounadi          #+#    #+#             */
/*   Updated: 2021/11/10 15:33:57 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int					i;
	unsigned long long	res;
	int					ng;

	i = 0;
	res = 0;
	ng = 1;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == 32))
		i++;
	if (str [i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			ng = -ng;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		if (res > LLONG_MAX)
		{
			if (ng == 1)
				return (-1);
			return (0);
		}
		i++;
	}
	return ((int)res * ng);
}
