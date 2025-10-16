/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <mounadi1337@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 02:56:06 by amounadi          #+#    #+#             */
/*   Updated: 2021/11/13 23:40:32 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	memory_to_hex(unsigned long long memory, int *a)
{
	char	*hex;
	int		len;

	hex = "0123456789abcdef";
	if (memory >= 16)
	{
		(*a)++;
		memory_to_hex(memory / 16, a);
	}
	if (memory < 16)
	{
		(*a)++;
		len = write (1, "0x", 2);
	}
	len = write(1, &(hex[memory % 16]), 1);
	(void)len;
}

int	ft_put_memory(void *addr)
{
	unsigned long long				memory;
	unsigned char					*str;
	int								a;

	str = addr;
	a = 0;
	memory = (unsigned long long)&str[0];
	memory_to_hex(memory, &a);
	return (a + 2);
}
