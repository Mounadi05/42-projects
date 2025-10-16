/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 21:13:40 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/18 22:08:50 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	ft_count(int nb, int *a)
{
	unsigned int	n;

	n = nb;
	if (nb < 0)
	{
		ft_putchar('-');
		n = -nb;
		(*a)++;
	}
	if (n > 9)
	{
		(*a)++;
		ft_count(n / 10, a);
		ft_putchar((n % 10) + '0');
	}
	else
	{
		(*a)++;
		ft_putchar(n + '0');
	}
}

int	ft_putint(int nb)
{
	int	a;

	a = 0;
	ft_count(nb, &a);
	return (a);
}

int	ft_putlongint(long int n)
{
	int	a;

	a = 0;
	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
		a++;
	}
	if (n > 9)
	{
		a++;
		ft_putlongint(n / 10);
		ft_putchar((n % 10) + '0');
	}
	else
	{
		a++;
		ft_putchar(n + '0');
	}
	return (a);
}
