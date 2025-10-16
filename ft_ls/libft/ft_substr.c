/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <mounadi1337@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:08:18 by amounadi          #+#    #+#             */
/*   Updated: 2021/11/10 15:33:51 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*str;
	size_t		y;

	if (!s)
		return (NULL);
	y = ft_strlen(s);
	if (start >= y)
		return (ft_strdup(""));
	if (y <= len)
		str = (char *)malloc(y + 1);
	else
		str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	y = 0;
	while (s[start] && len-- > 0)
		str[y++] = s[start++];
	str[y] = '\0';
	return (str);
}
