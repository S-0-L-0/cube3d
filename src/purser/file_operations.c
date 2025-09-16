/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:09 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:28:43 by edforte          ###   ########.fr       */
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

int	file_exists(char *path)
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
