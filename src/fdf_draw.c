/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:55:40 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/02 17:43:05 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

void	draw_map(t_list **map, t_data *img)
{
	t_list	*ptr;
	int		x;
	int		y;

	ptr = *map;
	y = 0;
	while (ptr)
	{
		x = 0;
		while (x < ptr->width)
		{
			if (x < (*map)->width - 1)
				drw_line(new_p(x, y, ptr, img), new_p(x + 1, y, ptr, img), img);
			if (y < (*map)->height - 1 && img->malloc_check != -1)
				drw_line(new_p(x, y, ptr, img), \
				new_p(x, y + 1, ptr->next, img), img);
			x ++;
			if (img->malloc_check == -1)
				return ;
		}
		y ++;
		ptr = ptr->next;
	}
}

void	drw_line(t_point point, t_point dest, t_data *img)
{
	t_point		delta;
	t_point		sign;
	int			error[2];
	float		*colorstep;

	delta_sign(&delta, point, dest, &sign);
	colorstep = NULL;
	colorstep = cstep(colorstep, &point, &dest, &delta);
	if (!colorstep)
		img->malloc_check = -1;
	error[0] = delta.x - delta.y;
	while ((point.x != dest.x || point.y != dest.y) && colorstep)
	{
		put_pixel(img, &point, colorstep);
		error[1] = error[0] * 2;
		if (error[1] > -delta.y)
			error[0] -= delta.y;
		if (error[1] > -delta.y)
			point.x += sign.x;
		if (error[1] < delta.x)
			error[0] += delta.x;
		if (error[1] < delta.x)
			point.y += sign.y;
	}
	free (colorstep);
}

void	delta_sign(t_point *dlt, t_point p, t_point dst, t_point *sign)
{
	dlt->x = ft_abs(dst.x - p.x);
	dlt->y = ft_abs(dst.y - p.y);
	if (p.x < dst.x)
		sign->x = 1;
	else
		sign->x = -1;
	if (p.y < dst.y)
		sign->y = 1;
	else
		sign->y = -1;
	if (p.color < dst.color)
		sign->color = 1;
	else
		sign->color = -1;
}

void	put_pixel(t_data *img, t_point *point, float *c_step)
{
	if (point->x < img->width && point->y < img->height && \
		point->x >= 0 && point->y >= 0)
	{
		mlx_put_pixel(img->instance, point->x, point->y, point->color);
		gradient_step(point, c_step);
	}
}
