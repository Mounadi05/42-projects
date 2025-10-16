/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <mounadi1337@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 12:02:37 by amounadi          #+#    #+#             */
/*   Updated: 2021/11/09 17:13:29 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	sr;
	size_t	dt;

	i = 0;
	j = 0;
	sr = 0;
	while (dst[i] != '\0')
		i++;
	dt = i;
	while (src[sr] != '\0')
		sr++;
	while (src[j] != '\0' && ((i + 1) < dstsize))
		dst[i++] = src[j++];
	dst[i] = '\0';
	if (i > dstsize)
		return (sr + dstsize);
	return (sr + dt);
}
