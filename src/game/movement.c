#include "../../includes/cub3d.h"

void	move_player(t_game *game, double dir_x, double dir_y)
{
	double	new_x;
	double	new_y;
	double	delta_x;
	double	delta_y;
	double	move_speed;

	move_speed = game->time.frame_time * 4.0;
	delta_x = dir_x * move_speed;
	delta_y = dir_y * move_speed;
	new_x = game->player.pos_x + delta_x;
	new_y = game->player.pos_y + delta_y;

	if (game->map.grid[(int)new_y][(int)game->player.pos_x] == '0')
		game->player.pos_y = new_y;
	if (game->map.grid[(int)game->player.pos_y][(int)new_x] == '0')
		game->player.pos_x = new_x;
}

void	rot_player(t_game *game, double rot_speed)
{
    double  old_dir_x;
    double  old_plane_x;

    old_dir_x = game->player.dir_x;
    game->player.dir_x = game->player.dir_x * cos(rot_speed) - game->player.dir_y * sin(rot_speed);
    game->player.dir_y = old_dir_x * sin(rot_speed) + game->player.dir_y * cos(rot_speed);
    
    old_plane_x = game->player.plane_x;
    game->player.plane_x = game->player.plane_x * cos(rot_speed) - game->player.plane_y * sin(rot_speed);
    game->player.plane_y = old_plane_x * sin(rot_speed) + game->player.plane_y * cos(rot_speed);
}

void	update_player(t_game *game)
{
	if (game->keys.w)
		move_player(game, game->player.dir_x, game->player.dir_y);
	if (game->keys.s)
		move_player(game, -game->player.dir_x, -game->player.dir_y);
	if (game->keys.a)
		move_player(game, -game->player.plane_x, -game->player.plane_y);
	if (game->keys.d)
		move_player(game, game->player.plane_x, game->player.plane_y);
	if (game->keys.left)
		rot_player(game, -(3.0 * game->time.frame_time));
	if (game->keys.right)
		rot_player(game, 3.0 * game->time.frame_time);
}
