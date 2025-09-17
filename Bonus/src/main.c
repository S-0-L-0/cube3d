/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:00:54 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:59:05 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	main(int argc, char **argv)
{
	t_game			game;
	t_parse_data	parse_data;

	ft_memset(&game, 0, sizeof(t_game));
	ft_memset(&parse_data, 0, sizeof(t_parse_data));
	if (parser(argc, argv, &game, &parse_data) != 0)
	{
		cleanup_parse_data(&parse_data);
		return (cleanup_game(&game), 1);
	}
	cleanup_parse_data(&parse_data);
	if (init_mlx(&game.mlx) != 0)
	{
		printf("Error\nFailed to initialize graphics\n");
		return (cleanup_game(&game), 1);
	}
	if (set_textures(&game) != 0)
	{
		free_mlx(&game.mlx);
		return (cleanup_game(&game), 1);
	}
	game_loop(&game);
	free_mlx(&game.mlx);
	cleanup_game(&game);
	return (0);
}
