/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:08:44 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 15:32:32 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
