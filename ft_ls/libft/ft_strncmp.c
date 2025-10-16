/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amounadi <amounadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 12:03:39 by amounadi          #+#    #+#             */
/*   Updated: 2024/11/14 23:50:49 by amounadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	else
		while (s1[i] && (unsigned char)s1[i] == (unsigned char)s2[i]
			&& i < (n - 1))
			i++;
	return ((unsigned char )s1[i] - (unsigned char)s2[i]);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strcasecmp(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (s1[0] && s1[0] == '.' && ft_strcasecmp(s1 + 1, s2) == 0)
		return (1);
	if (s2[0] && s2[0] == '.' && ft_strcasecmp(s1, s2 + 1) == 0)
		return (-1);
	if (s1[i] == '.')
		i++;
	if (s2[j] == '.')
		j++;
	while (s1[i] && (ft_tolower((unsigned char)s1[i])
			== ft_tolower((unsigned char)s2[j])))
	{
		i++;
		j++;
	}
	return ((unsigned char)ft_tolower(s1[i])
		- (unsigned char)ft_tolower(s2[j]));
}
