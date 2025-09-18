/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_boundary_detection.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:21 by edforte           #+#    #+#             */
/*   Updated: 2025/09/18 15:46:11 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	validate_map_start(t_parse_data *parse)
{
	if (parse->map_start_line == 0)
	{
		printf("Error\nNo map found in file\n");
		return (1);
	}
	return (0);
}

int	scan_map_end(t_parse_data *parse, int start, int *end)
{
	int	i;

	i = start;
	*end = start;
	while (parse->file_content[i])
	{
		if (is_empty_line(parse->file_content[i]))
		{
			break ;
		}
		if (!is_map_line(parse->file_content[i]))
		{
			printf("Error\nInvalid line in map: %s\n", parse->file_content[i]);
			return (1);
		}
		*end = i;
		i++;
	}
	return (0);
}

int	validate_map_height(int map_start, int map_end)
{
	int	height;

	height = map_end - map_start + 1;
	if (height < 3)
	{
		printf("Error\nMap too small (minimum 3 rows)\n");
		return (1);
	}
	if (height > 2147483641)
	{
		printf("Error\nMap too large (maximum 2147483641 rows)\n");
		printf("Current map has %d rows\n", height);
		return (1);
	}
	return (0);
}

int	find_map_boundaries(t_parse_data *parse, int *map_start, int *map_end)
{
	if (validate_map_start(parse) != 0)
		return (1);
	*map_start = parse->map_start_line;
	if (scan_map_end(parse, *map_start, map_end) != 0)
		return (1);
	return (validate_map_height(*map_start, *map_end));
}
