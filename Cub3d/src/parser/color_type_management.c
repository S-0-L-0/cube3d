/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_type_management.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:23:55 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:36:11 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_color_type_index(char *line)
{
	if (ft_strncmp(line, "F ", 2) == 0)
		return (0);
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (1);
	return (-1);
}

char	*get_color_identifier(int type_index)
{
	char	*identifiers[2];

	identifiers[0] = "Floor";
	identifiers[1] = "Ceiling";
	if (type_index >= 0 && type_index < 2)
		return (identifiers[type_index]);
	return (NULL);
}

int	*get_color_destination(int type_index, t_map *map)
{
	if (type_index == 0)
		return (map->floor_col);
	else if (type_index == 1)
		return (map->sky_col);
	return (NULL);
}

int	*get_color_loaded_flag(int type_index, t_parse_data *parse)
{
	if (type_index == 0)
		return (&parse->floor_set);
	else if (type_index == 1)
		return (&parse->ceiling_set);
	return (NULL);
}
