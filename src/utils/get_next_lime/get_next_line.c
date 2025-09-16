/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:22:42 by edforte           #+#    #+#             */
/*   Updated: 2024/05/07 11:39:35 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*trimmed_string(char *s1)
{
	char	*s2;
	int		i;

	i = 0;
	while (s1[i] && s1[i] != '\n')
		i ++;
	if (s1[i] == '\n')
		s2 = (char *)malloc((i + 2) * sizeof(char));
	else
		s2 = (char *)malloc((i + 1) * sizeof(char));
	i = 0;
	while (s1[i] && s1[i] != '\n')
	{
		s2[i] = s1[i];
		i ++;
	}
	if (s1[i] == '\n')
		s2[i++] = '\n';
	s2[i] = '\0';
	free(s1);
	return (s2);
}

static void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *) b;
	while (len-- > 0)
	{
		*ptr = (unsigned char) c;
		ptr ++;
	}
	return (b);
}

static char	*reading_lines(int fd, int *i, char *tmp)
{
	char	*newline;
	char	*str;

	str = NULL;
	newline = NULL;
	*i = read(fd, tmp, BUFFER_SIZE);
	if (*i == 0 || *i == -1)
		return (NULL);
	tmp[*i] = '\0';
	newline = ft_strchr(tmp, '\n');
	str = ft_strdup(tmp);
	while (*i != -1 && (!newline))
	{
		*i = read(fd, tmp, BUFFER_SIZE);
		if (*i == 0 || *i == -1)
			break ;
		tmp[*i] = '\0';
		str = ft_strjoin(str, tmp);
		newline = ft_strchr(tmp, '\n');
	}
	return (str);
}

void	get_line(int fd, int *i, char *buffer, char **str)
{
	char	*tmp;

	tmp = NULL;
	tmp = reading_lines(fd, i, buffer);
	if (!(*str) && tmp)
		*str = ft_strdup(tmp);
	else if (*str && tmp)
		*str = ft_strjoin(*str, tmp);
	ft_memset(buffer, 0, BUFFER_SIZE);
	if (tmp)
		free(tmp);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*str;
	int			i;
	char		*tmp;

	i = 1;
	str = NULL;
	tmp = NULL;
	if (buffer[0] != '\0')
		str = ft_strdup(buffer);
	ft_memset(buffer, 0, BUFFER_SIZE);
	if (!ft_strchr(str, '\n'))
	{
		get_line(fd, &i, buffer, &str);
	}
	if (!str || (i == 0 && !str) || i == -1)
		return (NULL);
	if (ft_strchr(str, '\n'))
		ft_strlcpy(buffer, (ft_strchr(str, '\n') + 1), (BUFFER_SIZE));
	tmp = trimmed_string(str);
	return (tmp);
}
