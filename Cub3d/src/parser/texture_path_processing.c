/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_path_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:25:00 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:35:16 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	*extract_and_clean_path(char *path_start, char *identifier)
{
	int		len;
	char	*result;

	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (*path_start == '\0')
	{
		printf("Error\nEmpty path for %s texture\n", identifier);
		return (NULL);
	}
	len = ft_strlen(path_start);
	while (len > 0 && (path_start[len - 1] == ' '
			|| path_start[len - 1] == '\t'))
		len--;
	path_start[len] = '\0';
	result = ft_strdup(path_start);
	if (!result)
	{
		printf("Error\nMemory allocation failed for %s texture path\n",
			identifier);
		return (NULL);
	}
	return (result);
}

int	store_texture_path(char *line, int type_index, t_map *map,
		t_parse_data *parse)
{
	char	*path_start;
	char	*cleaned_path;
	char	**dest_texture;
	int		*loaded_flag;

	loaded_flag = get_loaded_flag(type_index, parse);
	if (*loaded_flag)
	{
		printf("Error\nDuplicate %s texture definition\n",
			get_texture_identifier(type_index));
		return (1);
	}
	path_start = line + 3;
	cleaned_path = extract_and_clean_path(path_start,
			get_texture_identifier(type_index));
	if (!cleaned_path)
		return (1);
	dest_texture = get_texture_destination(type_index, map);
	*dest_texture = cleaned_path;
	*loaded_flag = 1;
	parse->texture_count++;
	return (0);
}

int	parse_texture_line(char *line, t_map *map, t_parse_data *parse)
{
	int		type_index;

	if (!line || !map || !parse)
		return (1);
	while (*line == ' ' || *line == '\t')
		line++;
	type_index = get_texture_type_index(line);
	if (type_index == -1)
		return (1);
	return (store_texture_path(line, type_index, map, parse));
}
