/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_type_management.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:25:02 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:34:01 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_texture_type_index(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (0);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (1);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (2);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (3);
	return (-1);
}

char	*get_texture_identifier(int type_index)
{
	char	*identifiers[4];

	identifiers[0] = "NO";
	identifiers[1] = "SO";
	identifiers[2] = "WE";
	identifiers[3] = "EA";
	if (type_index >= 0 && type_index < 4)
		return (identifiers[type_index]);
	return (NULL);
}

char	**get_texture_destination(int type_index, t_map *map)
{
	if (type_index == 0)
		return (&map->north_texture);
	else if (type_index == 1)
		return (&map->south_texture);
	else if (type_index == 2)
		return (&map->west_texture);
	else if (type_index == 3)
		return (&map->east_texture);
	return (NULL);
}

int	*get_loaded_flag(int type_index, t_parse_data *parse)
{
	if (type_index == 0)
		return (&parse->north_loaded);
	else if (type_index == 1)
		return (&parse->south_loaded);
	else if (type_index == 2)
		return (&parse->west_loaded);
	else if (type_index == 3)
		return (&parse->east_loaded);
	return (NULL);
}
