/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:22:46 by edforte           #+#    #+#             */
/*   Updated: 2024/05/06 16:42:55 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i ++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*strres;
	int		i;
	int		j;
	int		len_max;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	len_max = ((ft_strlen(s1) + ft_strlen(s2)) + 1);
	strres = (char *) malloc(len_max * sizeof(char));
	if (!strres)
		return (NULL);
	while (s1[i])
	{
		strres[i] = s1[i];
		i ++;
	}
	while (s2[j])
		strres[i++] = s2[j++];
	strres[i] = '\0';
	free((char *)s1);
	return (strres);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!src)
		return (0);
	while (src[i])
		i ++;
	if (dstsize == 0)
		return (i);
	while (src[j] && j < dstsize -1)
	{
		dst[j] = src[j];
		j ++;
	}
	dst[j] = '\0';
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*s2;

	s2 = (char *) s;
	i = 0;
	if (s == NULL)
		return (NULL);
	while (s2[i])
	{
		while (s2[i] == (char) c)
			return (&(s2[i]));
		i ++;
	}
	if (c == 0)
		return (&(s2[i]));
	return (NULL);
}

char	*ft_strdup(const char *s1)
{
	char	*s2;
	size_t	i;

	s2 = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!s2)
		return (0);
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i ++;
	}
	s2[i] = '\0';
	return (s2);
}
