/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loading.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:54 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:43:18 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	*get_texture_path(t_game *game, int index)
{
	if (index == 0)
		return (game->map.north_texture);
	else if (index == 1)
		return (game->map.south_texture);
	else if (index == 2)
		return (game->map.west_texture);
	else if (index == 3)
		return (game->map.east_texture);
	else if (index == 4)
		return ("assets/textures/cross.xpm");
	else if (index == 5)
		return ("assets/textures/torch20_1.xpm");
	else if (index == 6)
		return ("assets/textures/torch10_2.xpm");
	else if (index == 7)
		return ("assets/textures/torch40_3.xpm");
	else if (index == 8)
		return ("assets/textures/torch60_4.xpm");
	else if (index == 9)
		return ("assets/textures/torch70_5.xpm");
	return (NULL);
}

char	*get_error_texture_name(int index)
{
	if (index == 0)
		return ("north texture");
	else if (index == 1)
		return ("south texture");
	else if (index == 2)
		return ("west texture");
	else if (index == 3)
		return ("east texture");
	else if (index == 4)
		return ("cross");
	return ("torch");
}

void	cleanup_loaded_textures(t_game *game, int loaded_count)
{
	int	i;

	i = 0;
	while (i < loaded_count)
	{
		free_texture_struct(&game->textures[i], game->mlx.mlx);
		i++;
	}
}

int	load_single_texture(t_game *game, int index)
{
	char	*path;
	char	*error_name;

	path = get_texture_path(game, index);
	if (init_texture(&game->textures[index], path, game->mlx.mlx) != 0)
	{
		error_name = get_error_texture_name(index);
		printf("Error\nFailed to load %s: %s\n", error_name, path);
		cleanup_loaded_textures(game, index);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	return (0);
}
