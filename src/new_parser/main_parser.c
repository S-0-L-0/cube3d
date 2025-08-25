/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:08:44 by edforte           #+#    #+#             */
/*   Updated: 2025/08/25 15:43:25 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int parser(int argc, char **argv, t_game *game, t_parse_data *parse)
{
    int fd;
    
    /*
    ** FASE 1: VALIDAZIONI PRELIMINARI
    ** Controlla che ci sia esattamente un argomento e che abbia
    ** estensione .cub prima ancora di tentare di aprire il file
    */
    if (validate_arguments(argc) != 0)
        return (1);  // Errore già stampato dalla funzione
    
    /*
    ** FASE 2: APERTURA FILE
    ** Tenta di aprire il file, verifica che esista e sia leggibile
    ** Ritorna il file descriptor o -1 in caso di errore
    */
    fd = open_and_validate_file(argv[1]);
    if (fd == -1)
        return (1);  // Errore già stampato dalla funzione
    
    /*
    ** FASE 3: LETTURA FILE IN MEMORIA
    ** Usa get_next_line per leggere tutto il contenuto
    ** Salva ogni riga nell'array parse->file_content
    */
    if (read_file_content(fd, parse) != 0)
    {
        close(fd);  // Importante: chiudi il fd anche in caso di errore
        return (1);
    }
    close(fd);  // File letto completamente, possiamo chiuderlo
    
    /*
    ** FASE 4: PARSING CONFIGURAZIONE
    ** Scorre le righe cercando texture (NO/SO/WE/EA) e colori (F/C)
    ** Si ferma quando trova l'inizio della mappa
    ** Salva tutto in game->map (north_texture, south_texture, etc.)
    */
    if (parse_config_elements(parse, &game->map) != 0)
        return (1);
    
    /*
    ** FASE 5: VALIDAZIONE CONFIGURAZIONE
    ** Verifica che:
    ** - Tutte e 4 le texture siano state definite
    ** - Entrambi i colori (floor e ceiling) siano stati definiti
    ** - I file texture esistano e siano .xpm
    */
    if (validate_config_complete(&game->map, parse) != 0)
        return (1);
    
    /*
    ** FASE 6: PARSING MAPPA
    ** - Estrae la griglia dalla posizione salvata in parse->map_start_line
    ** - Normalizza la larghezza (padding con spazi)
    ** - Trova la posizione del player e la salva in game->player
    ** - Sostituisce il carattere player con '0' nella griglia
    */
    if (parse_map(parse, &game->map, &game->player) != 0)
        return (1);
    
    /*
    ** FASE 7: VALIDAZIONE MAPPA
    ** - Verifica caratteri validi (solo 0, 1, spazi, N/S/E/W)
    ** - Controlla che ci sia esattamente un player
    ** - Esegue flood-fill per verificare che la mappa sia chiusa
    **   (il player non deve poter raggiungere spazi vuoti)
    */
    if (validate_map(&game->map, &game->player) != 0)
        return (1);
    
    /*
    ** SUCCESSO!
    ** Il parsing è completato, la struttura game contiene:
    ** - map.grid: la mappa validata
    ** - map.*_texture: i percorsi delle texture
    ** - map.floor_color/ceiling_color: i colori RGB
    ** - player.pos_x/y: posizione del player
    ** - player.direction: orientamento iniziale (N/S/E/W)
    */
    return (0);
}
