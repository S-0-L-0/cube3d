typedef struct s_map {
    char **grid;          // La mappa come array 2D
    int width;            // Larghezza della mappa
    int height;           // Altezza della mappa
    char *north_texture;  // Percorso della texture nord
    char *south_texture;  // Percorso della texture sud
    char *west_texture;   // Percorso della texture ovest
    char *east_texture;   // Percorso della texture est
    int floor_color[3];   // Colore RGB del pavimento
    int ceiling_color[3]; // Colore RGB del soffitto
} t_map;

typedef struct s_player {
    double pos_x;         // Posizione X del giocatore
    double pos_y;         // Posizione Y del giocatore
    double dir_x;         // Direzione X del giocatore
    double dir_y;         // Direzione Y del giocatore
    double plane_x;       // Piano camera X
    double plane_y;       // Piano camera Y
    char direction;       // Direzione iniziale (N, S, E, W)
} t_player;

typedef struct s_ray {
    double camera_x;      // Coordinata X nello spazio della camera
    double dir_x;         // Direzione X del raggio
    double dir_y;         // Direzione Y del raggio
    int map_x;            // Coordinata X del quadrato della mappa
    int map_y;            // Coordinata Y del quadrato della mappa
    double side_dist_x;   // Distanza fino al prossimo lato X
    double side_dist_y;   // Distanza fino al prossimo lato Y
    double delta_dist_x;  // Distanza tra lati X successivi
    double delta_dist_y;  // Distanza tra lati Y successivi
    double perp_wall_dist; // Distanza perpendicolare al muro
    int step_x;           // Direzione del passo X (+1 o -1)
    int step_y;           // Direzione del passo Y (+1 o -1)
    int hit;              // Se è stato colpito un muro (1) o no (0)
    int side;             // Se è stato colpito un lato NS (0) o EW (1)
} t_ray;

typedef struct s_texture {
    void *img;            // Puntatore all'immagine
    char *addr;           // Indirizzo dei dati dell'immagine
    int bits_per_pixel;   // Bits per pixel
    int line_length;      // Lunghezza della linea
    int endian;           // Endianness
    int width;            // Larghezza della texture
    int height;           // Altezza della texture
} t_texture;

typedef struct s_mlx {
    void *mlx;            // Puntatore alla connessione MLX
    void *win;            // Puntatore alla finestra MLX
    void *img;            // Puntatore all'immagine
    char *addr;           // Indirizzo dei dati dell'immagine
    int bits_per_pixel;   // Bits per pixel
    int line_length;      // Lunghezza della linea
    int endian;           // Endianness
    int win_width;        // Larghezza della finestra
    int win_height;       // Altezza della finestra
} t_mlx;

typedef struct s_game {
    t_map map;            // Struttura della mappa
    t_player player;      // Struttura del giocatore
    t_mlx mlx;            // Struttura MLX
    t_texture textures[4]; // Array di texture (N, S, E, W)
} t_game;