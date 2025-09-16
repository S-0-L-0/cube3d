/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reading_control.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:13 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:31:32 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	read_file_content(int fd, t_parse_data *parse)
{
	if (allocate_file_content(parse) != 0)
		return (1);
	return (read_lines_from_file(fd, parse));
}

int	validate_texture_file(char *path, char *texture_name)
{
	int	len;

	if (!path)
	{
		printf("Error\n%s texture not defined\n", texture_name);
		return (1);
	}
	len = ft_strlen(path);
	if (len < 5 || ft_strncmp(&path[len - 4], ".xpm", 4) != 0)
	{
		printf("Error\n%s texture must be a .xpm file\n", texture_name);
		return (1);
	}
	if (!file_exists(path))
	{
		printf("Error\n%s texture file does not exist or cannot be read: %s\n",
			texture_name, path);
		return (1);
	}
	return (0);
}
