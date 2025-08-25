/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:11:56 by edforte           #+#    #+#             */
/*   Updated: 2025/08/23 15:24:20 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int parse_map(t_parse_data *parse, t_map *map, t_player *player)
{
    int map_start;
    int map_end;
    int i;
    
    /*
    ** TROVA I CONFINI DELLA MAPPA
    ** Delega a funzione helper per trovare dove inizia e finisce
    ** la mappa effettiva (escludendo linee vuote finali)
    */
    if (find_map_boundaries(parse, &map_start, &map_end) != 0)
        return (1);
    
    /*
    ** CONTROLLO CRITICO: NIENTE DOPO LA MAPPA!
    ** Dopo l'ultima riga della mappa, sono permessi SOLO:
    ** - Linee completamente vuote
    ** - Linee contenenti solo spazi e/o tab
    ** 
    ** La funzione is_empty_line() ritorna true per:
    ** - Stringhe vuote ("")
    ** - Stringhe con solo spazi ("     ")
    ** - Stringhe con solo tab ("\t\t")
    ** - Mix di spazi e tab ("  \t  \t  ")
    ** 
    ** Qualsiasi altro carattere causa errore perché la mappa
    ** DEVE essere l'ultimo elemento significativo del file
    */
    i = map_end + 1;
    while (parse->file_content[i])
    {
        if (!is_empty_line(parse->file_content[i]))
        {
            /*
            ** ERRORE: Contenuto non valido dopo la mappa
            ** La linea contiene caratteri diversi da spazi/tab
            ** Potrebbe essere una texture, un colore, o altro
            */
            printf("Error\nInvalid content after map: %s\n", 
                   parse->file_content[i]);
            printf("Map must be the last element in the file\n");
            return (1);
        }
        i++;
    }
    
    /*
    ** CALCOLA DIMENSIONI MAPPA
    ** height: numero di righe dalla prima all'ultima (incluse)
    ** width: verrà calcolato da normalize_map_width dopo
    */
    map->height = map_end - map_start + 1;
    
    /*
    ** ALLOCAZIONE GRIGLIA
    ** Allochiamo un array di puntatori a char
    ** +1 per il NULL terminator finale
    */
    map->grid = (char **)malloc(sizeof(char *) * (map->height + 1));
    if (!map->grid)
    {
        printf("Error\nMemory allocation failed for map grid\n");
        return (1);
    }
    
    /*
    ** COPIA RIGHE DELLA MAPPA
    ** Copiamo ogni riga dalla struttura parse alla griglia finale
    ** Le righe potrebbero avere lunghezze diverse per ora
    */
    i = 0;
    while (i < map->height)
    {
        /*
        ** DUPLICA LA RIGA
        ** Usiamo strdup per allocare e copiare la riga
        ** Ogni riga è indipendente e può essere liberata separatamente
        */
        map->grid[i] = ft_strdup(parse->file_content[map_start + i]);
        if (!map->grid[i])
        {
            printf("Error\nMemory allocation failed for map row %d\n", i);
            /*
            ** CLEANUP PARZIALE
            ** Libera le righe già allocate prima di ritornare errore
            */
            while (i > 0)
            {
                i--;
                free(map->grid[i]);
            }
            free(map->grid);
            map->grid = NULL;
            return (1);
        }
        i++;
    }
    
    /*
    ** NULL TERMINATOR
    ** Come per le stringhe, terminiamo l'array con NULL
    ** Questo permette di iterare con while(map->grid[i])
    */
    map->grid[map->height] = NULL;
    
    /*
    ** NORMALIZZA LARGHEZZA (PADDING)
    ** Rende tutte le righe della stessa lunghezza
    ** aggiungendo spazi dove necessario.
    ** 
    ** PERCHÉ PADDING E NON TRIMMING:
    ** 1. Griglia rettangolare = accesso sicuro grid[y][x]
    ** 2. Flood-fill più semplice senza controlli di bounds variabili
    ** 3. Raycasting uniforme su griglia rettangolare
    ** 4. Standard nel progetto cub3d
    */
    if (normalize_map_width(map) != 0)
    {
        /*
        ** CLEANUP IN CASO DI ERRORE
        */
        i = 0;
        while (map->grid[i])
        {
            free(map->grid[i]);
            i++;
        }
        free(map->grid);
        map->grid = NULL;
        return (1);
    }
    
    /*
    ** ESTRAI POSIZIONE PLAYER
    ** Cerca il player (N/S/E/W) nella mappa
    ** e salva la sua posizione e direzione.
    ** Il carattere del player viene sostituito con '0'
    ** per semplificare il rendering e la logica di gioco
    */
    if (extract_player_position(map, player) != 0)
    {
        /*
        ** CLEANUP IN CASO DI ERRORE
        */
        i = 0;
        while (map->grid[i])
        {
            free(map->grid[i]);
            i++;
        }
        free(map->grid);
        map->grid = NULL;
        return (1);
    }
    
    /*
    ** LOG DIMENSIONI FINALI (OPZIONALE - DEBUG)
    */
    /*
    printf("Map loaded: %d x %d\n", map->width, map->height);
    */
    
    /*
    ** SUCCESSO
    ** La mappa è stata:
    ** 1. Estratta dal file
    ** 2. Verificata (niente dopo di essa)
    ** 3. Normalizzata (tutte le righe stessa lunghezza)
    ** 4. Player trovato e posizionato
    ** 
    ** Ora è pronta per la validazione finale (flood-fill, etc.)
    */
    return (0);
}

int normalize_map_width(t_map *map)
{
    int     max_width;
    int     current_len;
    int     i;
    int     j;
    char    *new_row;
    
    /*
    ** TROVA LARGHEZZA MASSIMA
    ** Scorriamo tutte le righe per trovare la più lunga
    */
    max_width = 0;
    i = 0;
    while (i < map->height)
    {
        current_len = ft_strlen(map->grid[i]);
        if (current_len > max_width)
            max_width = current_len;
        i++;
    }
    
    /*
    ** SALVA LARGHEZZA NELLA STRUTTURA
    ** Questa sarà la larghezza uniforme di tutte le righe
    */
    map->width = max_width;
    
    /*
    ** CONTROLLO DIMENSIONE MINIMA
    ** Una mappa deve essere almeno 3x3 per essere valida
    ** (muri su tutti i lati + almeno uno spazio interno)
    */
    if (map->width < 3 || map->height < 3)
    {
        printf("Error\nMap too small (minimum 3x3)\n");
        return (1);
    }
    
    /*
    ** NORMALIZZA OGNI RIGA
    ** Ogni riga deve essere lunga esattamente max_width
    ** Aggiungiamo spazi alla fine se necessario
    */
    i = 0;
    while (i < map->height)
    {
        current_len = ft_strlen(map->grid[i]);
        
        /*
        ** SE LA RIGA È GIÀ DELLA LUNGHEZZA GIUSTA
        ** Non c'è niente da fare, passa alla prossima
        */
        if (current_len == max_width)
        {
            i++;
            continue;
        }
        
        /*
        ** ALLOCA NUOVA RIGA DELLA DIMENSIONE CORRETTA
        ** +1 per il terminatore '\0'
        */
        new_row = (char *)malloc(sizeof(char) * (max_width + 1));
        if (!new_row)
        {
            printf("Error\nMemory allocation failed during normalization\n");
            return (1);
        }
        
        /*
        ** COPIA I CARATTERI ESISTENTI
        */
        j = 0;
        while (j < current_len)
        {
            new_row[j] = map->grid[i][j];
            j++;
        }
        
        /*
        ** RIEMPI CON SPAZI FINO ALLA LARGHEZZA DESIDERATA
        ** Gli spazi rappresentano zone vuote/non accessibili della mappa
        */
        while (j < max_width)
        {
            new_row[j] = ' ';
            j++;
        }
        
        /*
        ** TERMINA LA STRINGA
        */
        new_row[j] = '\0';
        
        /*
        ** SOSTITUISCI LA VECCHIA RIGA CON QUELLA NUOVA
        ** Libera la vecchia e usa la nuova
        */
        free(map->grid[i]);
        map->grid[i] = new_row;
        
        i++;
    }
    
    /*
    ** SUCCESSO
    ** Tutte le righe ora hanno la stessa lunghezza
    ** La mappa è un rettangolo perfetto
    */
    return (0);
}

int find_map_boundaries(t_parse_data *parse, int *map_start, int *map_end)
{
    int i;
    int found_end;
    
    /*
    ** INIZIALIZZAZIONE
    ** map_start_line è stato già settato da parse_config_elements
    ** quando ha trovato la prima linea della mappa
    */
    if (parse->map_start_line == 0)
    {
        printf("Error\nNo map found in file\n");
        return (1);
    }
    
    /*
    ** IMPOSTA INIZIO MAPPA
    ** Usiamo il valore già trovato da parse_config_elements
    */
    *map_start = parse->map_start_line;
    
    /*
    ** TROVA FINE MAPPA
    ** La fine della mappa è l'ultima riga non vuota
    ** partendo da map_start_line
    */
    *map_end = *map_start;
    found_end = 0;
    i = *map_start;
    
    /*
    ** SCANSIONE PER TROVARE L'ULTIMA RIGA NON VUOTA
    ** Continuiamo a scorrere aggiornando map_end
    ** ogni volta che troviamo una riga non vuota
    */
    while (parse->file_content[i])
    {
        if (!is_empty_line(parse->file_content[i]))
        {
            *map_end = i;  /* Aggiorna l'ultima riga non vuota trovata */
            found_end = 1;
        }
        i++;
    }
    
    /*
    ** VERIFICA CHE ABBIAMO TROVATO ALMENO UNA RIGA DI MAPPA
    ** Se map_end è ancora uguale a map_start e non abbiamo
    ** trovato righe non vuote, c'è un problema
    */
    if (!found_end)
    {
        printf("Error\nMap appears to be empty\n");
        return (1);
    }
    
    /*
    ** VERIFICA CHE LA MAPPA NON SIA TROPPO GRANDE
    ** 
    ** LIMITE: 800 righe massimo
    ** 
    ** PERCHÉ 800:
    ** - File totale: max 999 righe (limite in read_file_content)
    ** - Config minima: ~6 righe (4 texture + 2 colori)
    ** - Margine sicurezza: ~193 righe per spazi e commenti
    ** - Memoria per 800x800: ~640KB (molto gestibile)
    ** - Stack flood-fill: sicuro con ricorsione fino a 800x800
    ** - Performance: ancora accettabile per il rendering
    ** 
    ** Con 800 abbiamo la CERTEZZA di non avere problemi di:
    ** - Memory overflow
    ** - Stack overflow nel flood-fill ricorsivo
    ** - Performance degradation eccessiva
    */
    if (*map_end - *map_start + 1 > 800)
    {
        printf("Error\nMap too large (maximum 800 rows)\n");
        printf("Current map has %d rows\n", *map_end - *map_start + 1);
        return (1);
    }
    
    /*
    ** VERIFICA CHE CI SIA ALMENO UNA RIGA
    ** Controllo di sicurezza aggiuntivo
    ** (non dovrebbe mai succedere, ma meglio essere sicuri)
    */
    if (*map_end < *map_start)
    {
        printf("Error\nInvalid map boundaries\n");
        return (1);
    }
    
    /*
    ** CONTROLLO DIMENSIONE MINIMA
    ** Una mappa deve essere almeno 3x3 per essere valida
    ** (muri su tutti i lati + almeno uno spazio interno)
    */
    if (*map_end - *map_start + 1 < 3)
    {
        printf("Error\nMap too small (minimum 3 rows)\n");
        return (1);
    }
    
    /*
    ** LOG INFORMATIVO (OPZIONALE - DEBUG)
    */
    /*
    printf("Map boundaries: lines %d to %d (%d rows total)\n", 
           *map_start, *map_end, *map_end - *map_start + 1);
    */
    
    /*
    ** SUCCESSO
    ** Abbiamo trovato dove inizia e finisce la mappa
    ** e verificato che rispetti tutti i limiti di sicurezza
    */
    return (0);
}

int extract_player_position(t_map *map, t_player *player)
{
    int     i;
    int     j;
    int     player_count;
    char    c;
    
    /*
    ** INIZIALIZZAZIONE
    ** player_count: conta quanti player troviamo (deve essere 1)
    ** Inizializziamo la posizione del player a -1 (non trovato)
    */
    player_count = 0;
    player->pos_x = -1.0;
    player->pos_y = -1.0;
    player->direction = 0;
    
    /*
    ** SCANSIONE DELLA MAPPA
    ** Cerchiamo il carattere del player (N/S/E/W)
    ** in tutta la griglia
    */
    i = 0;
    while (i < map->height)
    {
        j = 0;
        while (j < map->width)
        {
            c = map->grid[i][j];
            
            /*
            ** CONTROLLO SE È UN PLAYER
            */
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                /*
                ** CONTROLLO DUPLICATI
                */
                if (player_count > 0)
                {
                    printf("Error\nMultiple player starting positions found\n");
                    printf("First at [%d,%d], second at [%d,%d]\n",
                           (int)player->pos_y, (int)player->pos_x, i, j);
                    return (1);
                }
                
                /*
                ** SALVA POSIZIONE PLAYER
                ** Salviamo le coordinate ESATTE dalla griglia
                ** Il centraggio sarà fatto dopo, durante l'init del gioco
                */
                player->pos_x = (double)j;
                player->pos_y = (double)i;
                player->direction = c;
                player_count++;
                
                /*
                ** SOSTITUISCI CON SPAZIO VUOTO
                */
                map->grid[i][j] = '0';
            }
            j++;
        }
        i++;
    }
    
    /*
    ** VERIFICA PRESENZA PLAYER
    */
    if (player_count == 0)
    {
        printf("Error\nNo player starting position found in map\n");
        printf("Use N, S, E, or W to mark player position\n");
        return (1);
    }
    
    /*
    ** VERIFICA POSIZIONE VALIDA
    ** Il player non può essere sui bordi della mappa
    */
    if (player->pos_x == 0 || player->pos_x == map->width - 1 ||
        player->pos_y == 0 || player->pos_y == map->height - 1)
    {
        printf("Error\nPlayer cannot start on map border\n");
        return (1);
    }
    
    /*
    ** SUCCESSO
    ** Il player è stato trovato e la sua posizione ESATTA salvata
    ** Il centraggio (+0.5) sarà fatto durante l'inizializzazione del gioco
    */
    return (0);
}
