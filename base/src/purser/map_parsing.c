/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:26 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:46:37 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	parse_map(t_parse_data *parse, t_map *map, t_player *player)
{
	int	map_start;
	int	map_end;

	if (find_map_boundaries(parse, &map_start, &map_end) != 0)
		return (1);
	if (validate_content_after_map(parse, map_end) != 0)
		return (1);
	if (allocate_map_grid(map, map_end - map_start + 1) != 0)
		return (1);
	if (copy_map_rows(parse, map, map_start) != 0)
		return (1);
	if (normalize_map_width(map) != 0)
	{
		cleanup_map_grid(map);
		return (1);
	}
	if (extract_player_position(map, player) != 0)
	{
		cleanup_map_grid(map);
		return (1);
	}
	return (0);
}

int	parser(int argc, char **argv, t_game *game, t_parse_data *parse)
{
	int	fd;

	if (validate_arguments(argc, argv) != 0)
		return (1);
	fd = open_and_validate_file(argv[1]);
	if (fd == -1)
		return (1);
	if (read_file_content(fd, parse) != 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	if (parse_config_elements(parse, &game->map) != 0)
		return (1);
	if (validate_config_complete(&game->map, parse) != 0)
		return (1);
	if (parse_map(parse, &game->map, &game->player) != 0)
		return (1);
	if (validate_map(&game->map, &game->player) != 0)
		return (1);
	return (0);
}
