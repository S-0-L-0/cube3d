/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:23:51 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 13:53:45 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_duplicate_color(int type_index, t_parse_data *parse)
{
	int	*loaded_flag;

	loaded_flag = get_color_loaded_flag(type_index, parse);
	if (*loaded_flag)
	{
		printf("Error\nDuplicate %s color definition\n",
			get_color_identifier(type_index));
		return (1);
	}
	return (0);
}

int	process_color_values(char *color_values, int type_index, t_map *map,
		t_parse_data *parse)
{
	int	*dest_color;
	int	*loaded_flag;

	while (*color_values == ' ' || *color_values == '\t')
		color_values++;
	if (*color_values == '\0')
	{
		printf("Error\nEmpty RGB values for %s color\n",
			get_color_identifier(type_index));
		return (1);
	}
	dest_color = get_color_destination(type_index, map);
	if (extract_rgb_values(color_values, dest_color) != 0)
	{
		printf("Error\nInvalid RGB format for %s color\n",
			get_color_identifier(type_index));
		return (1);
	}
	loaded_flag = get_color_loaded_flag(type_index, parse);
	*loaded_flag = 1;
	return (0);
}

int	parse_color_line(char *line, t_map *map, t_parse_data *parse)
{
	int		type_index;
	char	*color_values;

	if (!line || !map || !parse)
		return (1);
	while (*line == ' ' || *line == '\t')
		line++;
	type_index = get_color_type_index(line);
	if (type_index == -1)
		return (1);
	if (check_duplicate_color(type_index, parse) == 1)
		return (1);
	color_values = line + 2;
	return (process_color_values(color_values, type_index, map, parse));
}
