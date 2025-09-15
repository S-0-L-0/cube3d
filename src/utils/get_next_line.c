/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:31:20 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 16:59:59 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	*get_next_line(int fd)
{
	static char	buffer[1024];
	static int	buffer_pos = 0;
	static int	buffer_size = 0;
	char		*line;
	int			line_len;
	char		c;

	line = NULL;
	line_len = 0;
	while (1)
	{
		if (buffer_pos >= buffer_size)
		{
			buffer_size = read(fd, buffer, 1024);
			buffer_pos = 0;
			if (buffer_size <= 0)
				break ;
		}
		c = buffer[buffer_pos++];
		line = realloc(line, line_len + 2);
		line[line_len++] = c;
		if (c == '\n')
			break ;
	}
	if (line)
		line[line_len] = '\0';
	return (line);
}
