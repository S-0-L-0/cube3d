/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:12:07 by edforte           #+#    #+#             */
/*   Updated: 2025/09/16 14:35:47 by edforte          ###   ########.fr       */
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

int	read_file_content(int fd, t_parse_data *parse)
{
	if (allocate_file_content(parse) != 0)
		return (1);
	return (read_lines_from_file(fd, parse));
}

int	is_empty_line(char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	is_map_line(char *line)
{
	int	i;
	int	has_map_char;

	if (!line)
		return (0);
	has_map_char = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != ' '
			&& line[i] != '\t' && line[i] != 'N' && line[i] != 'S'
			&& line[i] != 'E' && line[i] != 'W')
		{
			return (0);
		}
		if (line[i] == '0' || line[i] == '1' || line[i] == 'N'
			|| line[i] == 'S' || line[i] == 'E'
			|| line[i] == 'W')
		{
			has_map_char = 1;
		}
		i++;
	}
	return (has_map_char);
}
