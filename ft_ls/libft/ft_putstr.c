/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <mounadi1337@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 21:38:24 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/18 22:09:21 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(char *str)
{
	int	a;
	int	len;

	a = 0;
	if (!str)
	{
		len = write(1, "(null)", 6);
		return (6);
	}
	else
	{
		while (*str)
		{
			len = write(1, str++, 1);
			a++;
		}
	}
	(void)len;
	return (a);
}
