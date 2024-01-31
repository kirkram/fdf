/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/31 15:56:29 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "../lib/ft_printf/ft_printf.h"
#include "../lib/get_next_line/get_next_line.h"
#include "../include/fdf.h"
#include <math.h>
#include <fcntl.h>

int	main(int ac, char **av)
{
	t_list	*map;
	int		fd;

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
			return (1);
		map = fdf_reader(fd);
		close(fd);
		if (!map)
			return (1);
		init_and_draw(&map);
		ft_lstclear(&map, free);
	}
	return (EXIT_SUCCESS);
}

void	init_and_draw(t_list **map)
{
	t_data	*img;

	img = malloc(sizeof(t_data));
	zoom_calc(*map, img);
	img = init_window(img);
	if (!img || (mlx_image_to_window(img->mlx, img->img, 0, 0) < 0))
	{
		free (img);
		ft_lstclear(map, free);
		return ;
	}
	img->map = map;
	draw_map(img->map, img);
	mlx_loop_hook(img->mlx, &ft_hook_hub, img);
	mlx_scroll_hook(img->mlx, &my_scrollhook, img);
	mlx_loop(img->mlx);
	mlx_terminate(img->mlx);
	ft_lstclear(img->map, free);
	free (img);
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

t_data	*init_window(t_data *img)
{
	img->width = TRGT_W;
	img->height = TRGT_H;
	img->shift_x = -100;
	img->shift_y = 0;
	img->shift_z = 0.2;
	img->angle_x = 0;
	img->angle_y = 0;
	img->angle_z = 0;
	img->height_col = MAGENTA;
	img->mlx = mlx_init(img->width, img->height, "FDF", 0);
	if (!img->mlx)
		return (NULL);
	img->backg = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->backg)
		return (NULL);
	img->img = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->img || !img->backg)
		return (NULL);
	if (!put_background(img))
		return (NULL);
	return (img);
}

t_data	*put_background(t_data *img)
{
	int		x;
	int		y;

	if (mlx_image_to_window(img->mlx, img->backg, 0, 0) < 0)
		return (NULL);
	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			mlx_put_pixel(img->backg, x, y, 0x000000FF);
			x ++;
		}
		y ++;
	}
	mlx_put_string(img->mlx, "ZOOM: scroll | +/-", 6, 1);
	mlx_put_string(img->mlx, "TRANSLATE: wasd", 6, 19);
	mlx_put_string(img->mlx, "ROTATE: y/h i/k j/l", 6, 38);
	mlx_put_string(img->mlx, "CHANGE DEPTH: z/x", 6, 56);
	mlx_put_string(img->mlx, "Z COLOR: m/n", 6, 74);
	mlx_put_string(img->mlx, "ISOMETRIC/RESET: r", 6, 92);
	mlx_put_string(img->mlx, "TOP VIEW: 1", 6, 110);
	return (img);
}

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
			if (y < (*map)->height - 1)
				drw_line(new_p(x, y, ptr, img), \
				new_p(x, y + 1, ptr->next, img), img);
			x ++;
		}
		y ++;
		ptr = ptr->next;
	}
}

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

void	drw_line(t_point point, t_point dest, t_data *img)
{
	t_point		delta;
	t_point		sign;
	int			error;
	int			error_2;
	float		*colorstep;

	assign_delta(&delta, point, dest);
	vector_find_sign(point, dest, &sign);
	colorstep = NULL;
	colorstep = cstep(colorstep, &point, &dest, &delta);
	error = delta.x - delta.y;
	while (point.x != dest.x || point.y != dest.y)
	{
		put_pixel(img, &point, colorstep);
		error_2 = error * 2;
		if (error_2 > -delta.y)
			error -= delta.y;
		if (error_2 > -delta.y)
			point.x += sign.x;
		if (error_2 < delta.x)
			error += delta.x;
		if (error_2 < delta.x)
			point.y += sign.y;
	}
	free (colorstep);
}

void	assign_delta(t_point *delta, t_point point, t_point dest)
{
	delta->x = ft_abs(dest.x - point.x);
	delta->y = ft_abs(dest.y - point.y);
}

void	put_pixel(t_data *img, t_point *point, float *c_step)
{
	if (point->x < img->width && point->y < img->height && \
		point->x >= 0 && point->y >= 0)
	{
		mlx_put_pixel(img->img, point->x, point->y, point->color);
		gradient_step(point, c_step);
	}
}

float	*cstep(float *cstep, t_point *p, t_point *dst, t_point *dlt)
{
	float		draw_steps;

	cstep = malloc(4 * sizeof(float));
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

int ft_abs(int result)
{
	if (result < 0)
		result = -result;
	return (result);
}

void	vector_find_sign(t_point point, t_point dest, t_point *sign)
{
	if (point.x < dest.x)
		sign->x = 1;
	else
		sign->x = -1;
	if (point.y < dest.y)
		sign->y = 1;
	else
		sign->y = -1;
	if (point.color < dest.color)
		sign->color = 1;
	else
		sign->color = -1;
}
