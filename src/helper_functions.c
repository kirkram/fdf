/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 13:05:18 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/01 17:22:44 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

int	free_n_0(int *int_arr, char **str_arr, unsigned int *color_arr)
{
	int	i;

	i = 0;
	if (int_arr)
		free (int_arr);
	if (color_arr)
		free (color_arr);
	if (str_arr)
	{
		while (str_arr[i])
		{
			free (str_arr[i]);
			i ++;
		}
		free (str_arr);
	}
	return (0);
}

float	zoom_calc(t_list *map, t_data *img)
{
	int		scale_w;
	int		scale_h;
	int		width;
	int		height;

	width = map->width;
	height = map->height;
	img->zoom = 1;
	scale_w = TRGT_W / (width + 1) / 2;
	scale_h = TRGT_H / (height + 1) / 2;
	if (ft_abs(TRGT_H - width * scale_h) <= ft_abs(TRGT_W - height * scale_w))
		img->zoom *= scale_h;
	else
		img->zoom *= scale_w;
	return (img->zoom);
}

int	ft_abs(int result)
{
	if (result < 0)
		result = -result;
	return (result);
}

void	shift_outofbounds(t_list *map, t_data *img)
{
	t_list	*ptr;
	t_point	point;
	t_point	index;
	int		big_y;

	ptr = map;
	big_y = 0;
	index.y = 0;
	while (ptr)
	{
		index.x = 0;
		while (index.x < ptr->width)
		{
			p_bounds(&point, index, ptr, img);
			point = rotation_func(point, img);
			if (ft_abs(point.y) > ft_abs(big_y))
				big_y = point.y;
			index.x ++;
		}
		ptr = ptr->next;
		index.y ++;
	}
	if (big_y < 0 || big_y > 1080)
		img->shift_y -= (big_y - 300);
}

void	p_bounds(t_point *p, t_point index, t_list *ptr, t_data *img)
{
	p->x = index.x;
	p->y = index.y;
	p->z = ptr->int_array[index.x];
	p->x *= img->zoom;
	p->y *= img->zoom;
	p->z *= img->zoom;
	p->z *= img->shift_z;
}
