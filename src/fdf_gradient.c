/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_gradient.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:12:32 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/02 17:44:48 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

float	*cstep(float *cstep, t_point *p, t_point *dst, t_point *dlt)
{
	float		draw_steps;

	cstep = malloc(4 * sizeof(float));
	if (!cstep)
		return (NULL);
	draw_steps = dlt->y;
	if (dlt->x > dlt->y)
		draw_steps = dlt->x;
	if (draw_steps == 0)
		draw_steps ++;
	cstep[0] = (get_r(dst->color) - get_r(p->color)) / draw_steps;
	cstep[1] = (get_g(dst->color) - get_g(p->color)) / draw_steps;
	cstep[2] = (get_b(dst->color) - get_b(p->color)) / draw_steps;
	return (cstep);
}

void	gradient_step(t_point *point, float *colorstep)
{
	int			newcolor[4];
	int			i;

	newcolor[0] = get_r(point->color);
	newcolor[1] = get_g(point->color);
	newcolor[2] = get_b(point->color);
	newcolor[3] = get_a(point->color);
	i = -1;
	while (++i < 3)
		newcolor[i] = (int)round(newcolor[i] + colorstep[i]);
	i = 0;
	while (i < 3)
	{
		if (newcolor[i] < 0)
			newcolor[i] = 0;
		else if (newcolor[i] > 255)
			newcolor[i] = 255;
		i ++;
	}
	point->color = ((u_int32_t)newcolor[0] << 24) | \
	((u_int32_t)newcolor[1] << 16) | \
	((u_int32_t)newcolor[2] << 8) | (u_int32_t)newcolor[3];
}
