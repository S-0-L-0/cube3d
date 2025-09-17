/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configuration_validation.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:00 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 14:22:15 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	validate_config_vertical(int error, t_map *map)
{
	if (!map->north_texture)
	{
		printf("Error\nMissing North (NO) texture\n");
		error = 1;
	}
	else if (validate_texture_file(map->north_texture, "North") != 0)
		error = 1;
	if (!map->south_texture)
	{
		printf("Error\nMissing South (SO) texture\n");
		error = 1;
	}
	else if (validate_texture_file(map->south_texture, "South") != 0)
		error = 1;
	return (error);
}

int	validate_config_horizontal(int error, t_map *map)
{
	if (!map->west_texture)
	{
		printf("Error\nMissing West (WE) texture\n");
		error = 1;
	}
	else if (validate_texture_file(map->west_texture, "West") != 0)
		error = 1;
	if (!map->east_texture)
	{
		printf("Error\nMissing East (EA) texture\n");
		error = 1;
	}
	else if (validate_texture_file(map->east_texture, "East") != 0)
		error = 1;
	return (error);
}

int	validate_config_complete(t_map *map, t_parse_data *parse)
{
	int	error;

	error = 0;
	if (validate_config_vertical(error, map))
		return (1);
	if (validate_config_horizontal(error, map))
		return (1);
	if (!parse->floor_set)
	{
		printf("Error\nMissing Floor (F) color\n");
		error = 1;
	}
	if (!parse->ceiling_set)
	{
		printf("Error\nMissing Ceiling (C) color\n");
		error = 1;
	}
	return (error);
}
