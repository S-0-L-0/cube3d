/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_content_reading.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:04 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:29:36 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	allocate_file_content(t_parse_data *parse)
{
	parse->file_content = (char **)malloc(sizeof(char *) * 100000);
	if (!parse->file_content)
	{
		printf("Error\nMemory allocation failed\n");
		return (1);
	}
	return (0);
}

char	*remove_newline(char *line)
{
	int	len;

	len = 0;
	while (line[len])
		len++;
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

int	handle_file_too_large(char *line, t_parse_data *parse, int line_count)
{
	printf("Error\nFile too large (maximum 100000 lines)\n");
	free(line);
	while (line_count > 0)
	{
		line_count--;
		free(parse->file_content[line_count]);
	}
	free(parse->file_content);
	parse->file_content = NULL;
	return (1);
}

int	read_lines_from_file(int fd, t_parse_data *parse)
{
	char	*line;
	int		line_count;

	line_count = 0;
	line = get_next_line(fd);
	while (line != NULL && line_count < 100000)
	{
		line = remove_newline(line);
		parse->file_content[line_count] = line;
		line_count++;
		line = get_next_line(fd);
	}
	if (line != NULL)
		return (handle_file_too_large(line, parse, line_count));
	parse->file_content[line_count] = NULL;
	return (0);
}
