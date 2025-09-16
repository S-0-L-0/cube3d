/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_width_processing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:34 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:48:08 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"


int	normalize_map_width(t_map *map)
{
	int	max_width;

	max_width = find_max_width(map);
	map->width = max_width;
	if (validate_map_size(map) != 0)
		return (1);
	return (pad_all_rows(map, max_width));
}
