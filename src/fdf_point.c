/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_point.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:10:42 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/31 16:11:18 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

t_point	new_p(int x, int y, t_list *map, t_data *img)
{
	t_point	point;

	point.x = x;
	point.y = y;
	point.z = map->int_array[x];
	point.z *= img->shift_z;
	point.color = map->color_array[x];
	if (point.z != 0 && point.color == WHITE)
		point.color = height_to_color(point, img);
	point.x *= img->zoom;
	point.y *= img->zoom;
	point.z *= img->zoom;
	point = rotation_func(point, img);
	point.x += img->shift_x;
	point.y += img->shift_y;
	point.x += img->width / 2;
	point.y += img->height / 5;
	return (point);
}

unsigned int	height_to_color(t_point point, t_data *img)
{
	if (point.z > 0)
	{
		if (point.z > 6)
			point.color = YELLOW;
		else
			point.color = img->height_col;
	}
	if (point.z < 0)
	{
		if (img->height_col == MAGENTA)
			point.color = PURPLE;
		if (img->height_col == YELLOW)
			point.color = 0x0000FFFF;
	}
	return (point.color);
}

t_point	rotation_func(t_point point, t_data *img)
{
	t_point	temp;
	float	angle_45;
	float	angle_30;
	float	angle_1;

	angle_45 = (45 + img->angle_z) * 0.01745;
	angle_30 = (30 + img->angle_x) * 0.01745;
	temp.x = point.x * cos(angle_45) - point.y * sin(angle_45);
	temp.y = point.y * cos(angle_45) + point.x * sin(angle_45);
	point.x = temp.x;
	point.y = temp.y;
	temp.y = point.y * cos(angle_30) - point.z * sin(angle_30);
	temp.z = point.z * cos(angle_30) + point.y * sin(angle_30);
	point.y = temp.y;
	point.z = temp.z;
	if (img->angle_y != 0)
	{
		angle_1 = img->angle_y * (PI / 180);
		temp.x = point.x * cos(angle_1) + point.z * sin(angle_1);
		temp.z = -1 * point.x * sin(angle_1) + point.z * cos(angle_1);
		point.x = temp.x;
		point.z = temp.z;
	}
	return (point);
}
