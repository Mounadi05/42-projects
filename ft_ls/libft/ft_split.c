/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <mounadi1337@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 14:18:16 by amounadi          #+#    #+#             */
/*   Updated: 2021/11/09 18:58:13 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_lent(char const *s, char c)
{
	int	lent;

	lent = 0;
	while (s[lent] && s[lent] != c)
		lent++;
	return (lent);
}

static	int	ft_count_str(char const *str, char c)
{
	int	a;
	int	i;

	a = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		i = ft_lent(str, c);
		str += i;
		if (i)
			a++;
	}
	return (a);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		size;
	int		i;
	int		len;

	if (!s)
		return (NULL);
	size = ft_count_str(s, c);
	str = (char **)malloc(sizeof(char *) * (size + 1));
	i = -1;
	if (!str)
		return (NULL);
	while (++i < size)
	{
		while (*s == c)
			s++;
		len = ft_lent(s, c);
		str[i] = (char *)malloc(len + 1);
		if (!str[i])
			return (NULL);
		ft_strlcpy(str[i], s, len + 1);
		s += len;
	}
	str[i] = 0;
	return (str);
}
