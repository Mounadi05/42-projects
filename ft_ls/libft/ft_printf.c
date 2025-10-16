/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 15:46:58 by amounadi          #+#    #+#             */
/*   Updated: 2024/12/18 16:02:43 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_fix_norm(char c, va_list pa)
{
	int	a;

	a = 0;
	if (c == 'c')
		a = ft_putchar(va_arg(pa, int));
	else if (c == 's')
		a = ft_putstr(va_arg(pa, char *));
	else if (c == 'p')
		a = ft_put_memory(va_arg(pa, void *));
	else if (c == 'd')
		a = ft_putint(va_arg(pa, int));
	else if (c == 'i')
		a = ft_putint(va_arg(pa, int));
	else if (c == 'u')
		a = ft_putuint(va_arg(pa, unsigned int));
	else if (c == 'x')
		a = ft_puthexlow(va_arg(pa, int));
	else if (c == 'X')
		a = ft_puthexupper(va_arg(pa, int));
	else if (c == '%')
		a = ft_putchar('%');
	return (a);
}

int	ft_printf(const char *str, ...)
{
	va_list		pa;
	int			i;
	int			a;

	i = 0;
	a = 0;
	va_start(pa, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			if (str[i] == 'l' && str[i + 1] && str[++i] == 'd')
				a = a + ft_putlongint(va_arg(pa, long int));
			else
				a = a + ft_fix_norm(str[i], pa);
		}
		else
			a = a + ft_putchar(str[i]);
		i++;
	}
	va_end(pa);
	return (a);
}
