/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_hook_controls.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:13:49 by klukiano          #+#    #+#             */
/*   Updated: 2024/11/07 19:29:15 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	ft_hook_movement(t_data *img)
{
	if (mlx_is_key_down(img->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(img->mlx);
	if (mlx_is_key_down(img->mlx, MLX_KEY_A))
		img->shift_x -= 10;
	else if (mlx_is_key_down(img->mlx, MLX_KEY_D))
		img->shift_x += 10;
	if (mlx_is_key_down(img->mlx, MLX_KEY_W))
		img->shift_y -= 10;
	else if (mlx_is_key_down(img->mlx, MLX_KEY_S))
		img->shift_y += 10;
	if (mlx_is_key_down(img->mlx, MLX_KEY_Z) && img->shift_z > -0.5)
		img->shift_z -= 0.01;
	else if (mlx_is_key_down(img->mlx, MLX_KEY_X) && img->shift_z < 0.5)
		img->shift_z += 0.01;
	if (mlx_is_key_down(img->mlx, MLX_KEY_M))
		img->height_col = YELLOW;
	else if (mlx_is_key_down(img->mlx, MLX_KEY_N))
		img->height_col = MAGENTA;
	hook_rotation(img);
}

void	hook_rotation(t_data *img)
{
	if (mlx_is_key_down(img->mlx, MLX_KEY_J))
		img->angle_z += 1;
	else if (mlx_is_key_down(img->mlx, MLX_KEY_L))
		img->angle_z -= 1;
	if (mlx_is_key_down(img->mlx, MLX_KEY_I))
		img->angle_x += 1;
	else if (mlx_is_key_down(img->mlx, MLX_KEY_K))
		img->angle_x -= 1;
	if (mlx_is_key_down(img->mlx, MLX_KEY_Y))
		img->angle_y -= 1;
	else if (mlx_is_key_down(img->mlx, MLX_KEY_H))
		img->angle_y += 1;
}

void	hook_zoom_buttons(t_data *img)
{
	if (mlx_is_key_down(img->mlx, MLX_KEY_KP_ADD))
	{
		if (img->width < 100)
			img->zoom += 1;
		else
			img->zoom += 0.5;
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_KP_SUBTRACT))
	{
		if (img->zoom >= 1)
		{
			if (img->width < 100)
				img->zoom -= 1;
			else
				img->zoom -= 0.5;
		}
	}
}

void	my_scrollhook(double xdelta, double ydelta, void *param)
{
	t_data	*img;

	(void) xdelta;
	img = param;
	if (ydelta > 0)
	{
		img->zoom += 1;
		redraw_image(img);
	}
	else if (ydelta < 0)
	{
		if (img->zoom >= 1)
			img->zoom -= 1;
		redraw_image(img);
	}
}

void	hook_reset_image(t_data *img)
{
	if (mlx_is_key_down(img->mlx, MLX_KEY_1))
	{
		img->shift_x = -400;
		img->shift_y = 150;
		img->shift_z = 0.2;
		img->angle_x = -30;
		img->angle_y = 0;
		img->angle_z = -45;
		img->zoom = zoom_calc(*img->map, img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_R))
	{
		img->shift_x = -100;
		img->shift_y = 0;
		img->shift_z = 0.2;
		img->angle_x = 0;
		img->angle_y = 0;
		img->angle_z = 0;
		img->zoom = zoom_calc(*img->map, img);
	}
}
