/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_parsing_utilities.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:18 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 16:48:28 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
			&& line[i] != 'E' && line[i] != 'W' && line[i] != 'D')
		{
			return (0);
		}
		if (line[i] == '0' || line[i] == '1' || line[i] == 'N'
			|| line[i] == 'S' || line[i] == 'E'
			|| line[i] == 'W' || line[i] != 'D')
		{
			has_map_char = 1;
		}
		i++;
	}
	return (has_map_char);
}

int	process_config_line(t_parse_data *parse, t_map *map, int line_index)
{
	int	ret;

	if (is_empty_line(parse->file_content[line_index]))
		return (0);
	if (is_map_line(parse->file_content[line_index]))
	{
		printf("\n");
		parse->map_start_line = line_index;
		return (0);
	}
	ret = parse_texture_line(parse->file_content[line_index], map, parse);
	if (ret == 0)
		return (0);
	ret = parse_color_line(parse->file_content[line_index], map, parse);
	if (ret == 0)
		return (0);
	printf("Error\nInvalid configuration line: %s\n",
		parse->file_content[line_index]);
	return (1);
}

int	parse_config_elements(t_parse_data *parse, t_map *map)
{
	int	i;
	int	ret;

	i = 0;
	parse->map_start_line = -1;
	while (parse->file_content[i])
	{
		ret = process_config_line(parse, map, i);
		if (ret == 1)
			return (1);
		if (parse->map_start_line != -1)
			break ;
		i++;
	}
	if (parse->map_start_line == -1)
	{
		printf("Error\nNo map found in file\n");
		return (1);
	}
	return (0);
}
