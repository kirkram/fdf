/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_hook_control_hub.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 12:59:31 by klukiano          #+#    #+#             */
/*   Updated: 2024/11/07 19:29:13 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	ft_hook_hub(void *param)
{
	t_data	*img;
	t_data	prev;

	img = param;
	prev = *img;
	hook_reset_image(img);
	hook_zoom_buttons(img);
	ft_hook_movement(img);
	check_changes_to_img(&prev, img);
}

void	check_changes_to_img(t_data *prev, t_data *img)
{
	if (\
	prev->angle_x != img->angle_x || \
	prev->angle_y != img->angle_y || \
	prev->angle_z != img->angle_z || \
	prev->height_col != img->height_col || \
	prev->shift_x != img->shift_x || \
	prev->shift_y != img->shift_y || \
	prev->shift_z != img->shift_z || \
	prev->zoom != img->zoom)
		redraw_image(img);
}

t_data	*redraw_image(t_data *img)
{
	mlx_delete_image(img->mlx, img->instance);
	img->instance = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->instance)
	{
		mlx_close_window(img->mlx);
		return (NULL);
	}
	if (mlx_image_to_window(img->mlx, img->instance, 0, 0) < 0)
	{
		mlx_close_window(img->mlx);
		return (NULL);
	}
	draw_map(img->map, img);
	return (img);
}
