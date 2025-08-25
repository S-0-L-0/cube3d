/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map_validation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:12:01 by edforte           #+#    #+#             */
/*   Updated: 2025/08/25 09:50:04 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int validate_map(t_map *map, t_player *player)
{
    /*
    ** NOTA: extract_player_position() ha già:
    ** - Verificato che ci sia esattamente un player
    ** - Sostituito N/S/E/W con '0'
    ** - Salvato posizione in player->pos_x/y
    */
    
    /*
    ** STEP 1: VALIDAZIONE CARATTERI (NECESSARIO!)
    ** Verifica che TUTTI i caratteri siano validi
    ** Questo NON è stato fatto prima!
    */
    if (validate_map_characters(map) != 0)
        return (1);
    
    /*
    ** STEP 2: CONTROLLO BORDI (OPZIONALE)
    ** Ridondante con flood-fill ma dà errori più chiari
    ** Puoi commentarlo se preferisci solo flood-fill
    */

    if (check_map_borders(map) != 0)
        return (1);
    
    /*
    ** STEP 3: FLOOD-FILL CHECK (ESSENZIALE!)
    ** Verifica che il player non possa raggiungere spazi vuoti
    ** Questo copre anche il controllo dei bordi
    */
    if (flood_fill_check(map, player) != 0)
        return (1);
    
    return (0);
}

int validate_map_characters(t_map *map)
{
    int i;
    int j;
    char c;
    
    /*
    ** SCANSIONE COMPLETA DELLA GRIGLIA
    ** Controlliamo ogni carattere della mappa
    ** 
    ** CARATTERI VALIDI (dopo la sostituzione del player):
    ** - '0': spazio percorribile
    ** - '1': muro
    ** - ' ': spazio vuoto/void
    ** 
    ** NOTA: N/S/E/W sono già stati sostituiti con '0'
    ** da extract_player_position()
    */
    i = 0;
    while (i < map->height)
    {
        j = 0;
        while (j < map->width)
        {
            c = map->grid[i][j];
            
            /*
            ** VERIFICA CARATTERE VALIDO
            ** Solo 0, 1 e spazio sono permessi
            */
            if (c != '0' && c != '1' && c != ' ')
            {
                /*
                ** CARATTERE INVALIDO TROVATO
                ** Stampa informazioni dettagliate per debug
                */
                printf("Error\nInvalid character '%c' in map at position [%d,%d]\n",
                       c, i, j);
                
                /*
                ** SUGGERIMENTO PER L'UTENTE
                */
                if (c == '\t')
                    printf("Tab characters should be converted to spaces\n");
                else if (c >= 'A' && c <= 'Z' && c != 'N' && c != 'S' && 
                         c != 'E' && c != 'W')
                    printf("Only N, S, E, W are valid letters (for player spawn)\n");
                else
                    printf("Valid characters are: 0 (empty), 1 (wall), space (void)\n");
                
                return (1);
            }
            
            j++;
        }
        i++;
    }
    
    /*
    ** SUCCESSO
    ** Tutti i caratteri sono validi
    */
    return (0);
}

int flood_fill_check(t_map *map, t_player *player)
{
    int **visited;
    int result;
    
    /*
    ** ALLOCAZIONE MATRICE VISITED
    ** Usa la funzione helper per allocare
    */
    visited = allocate_2d_int_array(map->height, map->width);
    if (!visited)
    {
        printf("Error\nMemory allocation failed for flood fill\n");
        return (1);
    }
    
    /*
    ** ESEGUI FLOOD FILL
    ** Partiamo dalla posizione del player
    */
    result = flood_fill_recursive(map, visited, 
                                  (int)player->pos_x, 
                                  (int)player->pos_y);
    
    /*
    ** CLEANUP MEMORIA - VERSIONE CORRETTA
    ** Usa la funzione helper invece di farlo manualmente
    */
    free_2d_int_array(visited, map->height);
    visited = NULL;  /* Buona pratica anche se sta per uscire dallo scope */
    
    /*
    ** VERIFICA RISULTATO
    */
    if (result != 0)
    {
        printf("Error\nMap is not properly closed\n");
        printf("Player can reach void spaces or map boundaries\n");
        return (1);
    }
    
    return (0);
}

int flood_fill_recursive(t_map *map, int **visited, int x, int y)
{
    /*
    ** CONTROLLO LIMITI MAPPA
    ** Se siamo usciti dai confini della mappa,
    ** significa che c'è un'apertura!
    */
    if (x < 0 || x >= map->width || y < 0 || y >= map->height)
    {
        /*
        ** ERRORE: USCITI DALLA MAPPA
        ** Il player può raggiungere il bordo esterno
        */
        return (1);
    }
    
    /*
    ** CONTROLLO SE GIÀ VISITATO
    ** Evitiamo cicli infiniti
    */
    if (visited[y][x])
        return (0);  /* Già controllato, OK */
    
    /*
    ** MARCA COME VISITATO
    ** Prima di controllare il contenuto
    */
    visited[y][x] = 1;
    
    /*
    ** CONTROLLO CONTENUTO CELLA
    */
    char cell = map->grid[y][x];
    
    /*
    ** SE È UNO SPAZIO VUOTO
    ** Il player ha raggiunto un void/spazio!
    ** La mappa non è chiusa correttamente
    */
    if (cell == ' ')
    {
        /*
        ** ERRORE: RAGGIUNTO SPAZIO VUOTO
        */
        printf("Error\nPlayer can reach void space at position [%d,%d]\n", 
               y, x);
        return (1);
    }
    
    /*
    ** SE È UN MURO
    ** Stop, non possiamo attraversarlo
    */
    if (cell == '1')
        return (0);  /* Muro, OK */
    
    /*
    ** SE È UNO SPAZIO PERCORRIBILE ('0')
    ** Continua l'esplorazione nelle 4 direzioni
    ** 
    ** NOTA: A questo punto, cell può essere solo '0'
    ** perché abbiamo già controllato ' ' e '1'
    ** e validate_map_characters() ha verificato che
    ** non ci siano altri caratteri
    */
    
    /*
    ** ESPLORAZIONE RICORSIVA
    ** Controlliamo le 4 direzioni cardinali
    ** Se una qualsiasi ritorna errore, propaghiamo l'errore
    */
    
    /* DESTRA */
    if (flood_fill_recursive(map, visited, x + 1, y) != 0)
        return (1);
    
    /* SINISTRA */
    if (flood_fill_recursive(map, visited, x - 1, y) != 0)
        return (1);
    
    /* GIÙ */
    if (flood_fill_recursive(map, visited, x, y + 1) != 0)
        return (1);
    
    /* SU */
    if (flood_fill_recursive(map, visited, x, y - 1) != 0)
        return (1);
    
    /*
    ** SUCCESSO
    ** Questa cella e tutte quelle raggiungibili da qui sono OK
    */
    return (0);
}

int check_map_borders(t_map *map)
{
    int i;
    int j;
    char c;
    
    /*
    ** CONTROLLO PRIMA RIGA
    ** Tutti i caratteri devono essere '1' o ' '
    */
    i = 0;
    j = 0;
    while (j < map->width)
    {
        c = map->grid[0][j];
        if (c == '0')
        {
            printf("Error\nMap not closed: empty space '0' on top border at column %d\n", j);
            return (1);
        }
        j++;
    }
    
    /*
    ** CONTROLLO ULTIMA RIGA
    */
    i = map->height - 1;
    j = 0;
    while (j < map->width)
    {
        c = map->grid[i][j];
        if (c == '0')
        {
            printf("Error\nMap not closed: empty space '0' on bottom border at column %d\n", j);
            return (1);
        }
        j++;
    }
    
    /*
    ** CONTROLLO PRIMA COLONNA
    */
    i = 0;
    while (i < map->height)
    {
        c = map->grid[i][0];
        if (c == '0')
        {
            printf("Error\nMap not closed: empty space '0' on left border at row %d\n", i);
            return (1);
        }
        i++;
    }
    
    /*
    ** CONTROLLO ULTIMA COLONNA
    */
    j = map->width - 1;
    i = 0;
    while (i < map->height)
    {
        c = map->grid[i][j];
        if (c == '0')
        {
            printf("Error\nMap not closed: empty space '0' on right border at row %d\n", i);
            return (1);
        }
        i++;
    }
    
    /*
    ** NOTA: Permettiamo spazi ' ' sui bordi
    ** perché rappresentano void/zone non accessibili
    ** Il flood-fill verificherà che il player non possa raggiungerli
    */
    
    /*
    ** SUCCESSO
    ** I bordi non contengono spazi percorribili ('0')
    */
    return (0);
}
