/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:12:17 by edforte           #+#    #+#             */
/*   Updated: 2025/09/02 12:45:55 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void cleanup_parse_data(t_parse_data *parse)
{
    /*
    ** CONTROLLO NULL
    ** Se la struttura è NULL, non c'è niente da pulire
    */
    if (!parse)
        return;
    
    /*
    ** LIBERA FILE_CONTENT
    ** Usa free_split che è una funzione generica
    ** per liberare array di stringhe NULL-terminated
    ** Non duplichiamo il codice!
    */
    if (parse->file_content)
    {
        free_split(parse->file_content);
        parse->file_content = NULL;
    }
    
    /*
    ** RESET FLAGS E CONTATORI
    ** Non strettamente necessario se la struttura
    ** sta per essere distrutta, ma è buona pratica
    ** per evitare uso accidentale di valori sporchi
    */
    parse->map_start_line = -1;
    parse->texture_count = 0;
    parse->floor_set = 0;
    parse->ceiling_set = 0;
    parse->north_loaded = 0;
    parse->south_loaded = 0;
    parse->east_loaded = 0;
    parse->west_loaded = 0;
    
    /*
    ** NOTA IMPORTANTE:
    ** parse_data è di solito allocata sullo stack nel main:
    ** t_parse_data parse_data;  (NON t_parse_data *parse_data)
    ** 
    ** Quindi NON facciamo free(parse) perché:
    ** 1. Non è stata allocata con malloc
    ** 2. Verrà automaticamente liberata quando esce dallo scope
    ** 
    ** Se fosse allocata dinamicamente:
    ** t_parse_data *parse_data = malloc(sizeof(t_parse_data));
    ** Allora dovremmo fare free(parse) alla fine
    */
}

void cleanup_game(t_game *game)
{
    int i;
    
    /*
    ** CONTROLLO NULL
    */
    if (!game)
        return;
    
    /*
    ** CLEANUP MAPPA
    ** La mappa contiene la griglia e i path delle texture
    */
    
    /* LIBERA GRIGLIA MAPPA */
    if (game->map.grid)
    {
        i = 0;
        while (game->map.grid[i])
        {
            free(game->map.grid[i]);
            game->map.grid[i] = NULL;
            i++;
        }
        free(game->map.grid);
        game->map.grid = NULL;
    }
    
    /*
    ** LIBERA PATH TEXTURE
    ** Ogni path è una stringa allocata dinamicamente
    */
    if (game->map.north_texture)
    {
        free(game->map.north_texture);
        game->map.north_texture = NULL;
    }
    
    if (game->map.south_texture)
    {
        free(game->map.south_texture);
        game->map.south_texture = NULL;
    }
    
    if (game->map.east_texture)
    {
        free(game->map.east_texture);
        game->map.east_texture = NULL;
    }
    
    if (game->map.west_texture)
    {
        free(game->map.west_texture);
        game->map.west_texture = NULL;
    }
    
    /*
    ** RESET DIMENSIONI MAPPA
    */
    game->map.width = 0;
    game->map.height = 0;
    
    /*
    ** RESET COLORI
    ** I colori sono array statici, quindi solo reset valori
    */
    i = 0;
    while (i < 3)
    {
        game->map.floor_color[i] = 0;
        game->map.ceiling_color[i] = 0;
        i++;
    }
    
    /*
    ** RESET PLAYER
    ** Il player non ha memoria allocata dinamicamente,
    ** solo valori da resettare
    */
    game->player.pos_x = 0.0;
    game->player.pos_y = 0.0;
    game->player.dir_x = 0.0;
    game->player.dir_y = 0.0;
    game->player.plane_x = 0.0;
    game->player.plane_y = 0.0;
    game->player.direction = 0;
    
    /*
    ** CLEANUP MLX (SE INIZIALIZZATO)
    ** Questo dipende da come è stata inizializzata MLX
    ** Di solito non va fatto qui ma alla chiusura del programma
    */
    /* 
    ** if (game->mlx.img)
    **     mlx_destroy_image(game->mlx.mlx, game->mlx.img);
    ** if (game->mlx.win)
    **     mlx_destroy_window(game->mlx.mlx, game->mlx.win);
    ** if (game->mlx.mlx)
    **     mlx_destroy_display(game->mlx.mlx);
    */
    
    /*
    ** CLEANUP TEXTURES (SE CARICATE)
    ** Le texture MLX vanno liberate se sono state caricate
    */
    /*
    ** i = 0;
    ** while (i < 4)
    ** {
    **     if (game->textures[i].img)
    **         mlx_destroy_image(game->mlx.mlx, game->textures[i].img);
    **     i++;
    ** }
    */
    
    /*
    ** NOTA: game è di solito allocato sullo stack
    ** quindi non facciamo free(game)
    */
}

void cleanup_config(t_game *game)
{
    int i;
    
    /*
    ** CONTROLLO NULL
    */
    if (!game)
        return;
    
    /*
    ** QUESTA FUNZIONE PULISCE SOLO MAP E PLAYER
    ** Non tocca MLX o textures caricate
    ** Utile quando il parsing fallisce prima dell'init grafico
    */
    
    /*
    ** LIBERA GRIGLIA MAPPA
    */
    if (game->map.grid)
    {
        i = 0;
        /*
        ** LIBERA OGNI RIGA
        ** La griglia è NULL-terminated
        */
        while (game->map.grid[i])
        {
            free(game->map.grid[i]);
            game->map.grid[i] = NULL;
            i++;
        }
        /*
        ** LIBERA ARRAY DI PUNTATORI
        */
        free(game->map.grid);
        game->map.grid = NULL;
    }
    
    /*
    ** LIBERA PATH TEXTURE
    ** Solo i path, non le texture MLX caricate
    */
    if (game->map.north_texture)
    {
        free(game->map.north_texture);
        game->map.north_texture = NULL;
    }
    
    if (game->map.south_texture)
    {
        free(game->map.south_texture);
        game->map.south_texture = NULL;
    }
    
    if (game->map.east_texture)
    {
        free(game->map.east_texture);
        game->map.east_texture = NULL;
    }
    
    if (game->map.west_texture)
    {
        free(game->map.west_texture);
        game->map.west_texture = NULL;
    }
    
    /*
    ** RESET DIMENSIONI E COLORI
    */
    game->map.width = 0;
    game->map.height = 0;
    
    i = 0;
    while (i < 3)
    {
        game->map.floor_color[i] = 0;
        game->map.ceiling_color[i] = 0;
        i++;
    }
    
    /*
    ** RESET PLAYER
    ** Solo reset valori, nessuna memoria da liberare
    */
    game->player.pos_x = 0.0;
    game->player.pos_y = 0.0;
    game->player.dir_x = 0.0;
    game->player.dir_y = 0.0;
    game->player.plane_x = 0.0;
    game->player.plane_y = 0.0;
    game->player.direction = 0;
}

void free_map_grid(t_map *map)
{
    int i;
    
    /*
    ** CONTROLLO NULL
    */
    if (!map || !map->grid)
        return;
    
    /*
    ** LIBERA OGNI RIGA DELLA GRIGLIA
    ** La griglia è un array 2D di caratteri
    ** Ogni riga è allocata separatamente
    */
    i = 0;
    while (map->grid[i])
    {
        /*
        ** LIBERA LA RIGA
        ** Ogni riga è una stringa allocata dinamicamente
        */
        free(map->grid[i]);
        map->grid[i] = NULL;
        i++;
    }
    
    /*
    ** LIBERA L'ARRAY DI PUNTATORI ALLE RIGHE
    */
    free(map->grid);
    map->grid = NULL;
    
    /*
    ** RESET DIMENSIONI
    ** Importante per indicare che la griglia non è più valida
    */
    map->width = 0;
    map->height = 0;
    
    /*
    ** NOTA: Questa funzione libera SOLO la griglia
    ** Non tocca texture paths o colori
    ** È utile per liberare solo la mappa senza toccare
    ** il resto della configurazione
    */
}