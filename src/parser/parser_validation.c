/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:11:29 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 16:53:10 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	validate_arguments(int argc, char **argv)
{
	int	len;

	if (argc != 2)
	{
		if (argc < 2)
			printf("Error\nNo map file provided\n");
		else
			printf("Error\nToo many arguments\n");
		printf("Usage: ./cub3d <map.cub>\n");
		return (1);
	}
	len = ft_strlen(argv[1]);
	if (len < 5 || ft_strncmp(&argv[1][len - 4], ".cub", 4) != 0)
	{
		printf("Error\nMap file must have .cub extension\n");
		return (1);
	}
	return (0);
}

int	open_and_validate_file(char *filepath)
{
	int	fd;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		printf("Error\nCannot open file: %s\n", filepath);
		return (-1);
	}
	return (fd);
}

static int	file_exists(char *path)
{
	int	fd;

	if (!path)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	close(fd);
	return (1);
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
