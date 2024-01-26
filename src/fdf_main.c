/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/26 15:33:47 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../lib/libft/libft.h"
#include "../lib/ft_printf/ft_printf.h"
#include "../lib/get_next_line/get_next_line.h"
#include "../lib/MLX42/include/MLX42/MLX42.h"
#include "../lib/MLX42/include/MLX42/MLX42_Int.h"
#include "../include/fdf.h"
#include <math.h>
#include <fcntl.h>

t_list	*fdf_reader(int fd);
int		check_and_del_newline(char *l);
int		is_cell_colored(char *str);
int		is_valid_hex(char *str);
static	void ft_hook(void* param);
t_list	*ft_pointnew(void *content);
void	draw_and_move_00(t_list **map);

void	init_and_draw(t_list **map);
void	draw_map(t_list **map, t_data *img);
t_point	new_p(int x, int y, t_list *map, t_data *img);
void	drw_line(t_point point, t_point dest, t_data *img);
int		ft_abs(int result);
void	vector_find_sign(t_point point, t_point dest, t_point *sign);
t_point	apply_camera(t_point point, t_data *img);
void	zoom_calc(t_list *map, t_data *img);
t_data	*init_window(t_data *img);
t_point	rotation_iso(t_point point);

void	draw_line(t_point point, t_point dest, t_data *img);

t_list	*fdf_reader(int fd);

int	main(int ac, char **av)
{
	t_list	*map;
	int		fd;
	//int		i;

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
			return (1);
		map = fdf_reader(fd);
		if (!map)
			return (1);
		init_and_draw(&map);
	}
	ft_lstclear(&map, free);
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
	draw_map(map, img);
	mlx_loop_hook(img->mlx, ft_hook, NULL);
	mlx_loop(img->mlx);
	mlx_terminate(img->mlx);
	ft_printf("\n");
}

void	zoom_calc(t_list *map, t_data *img)
{
	double	scale_w;
	double	scale_h;
	int		width;
	int		height;

	width = map->width;
	height = map->height;
	scale_w = TRGT_W / (width + 1) / 2;
	scale_h = TRGT_H / (height + 1) / 2;
	if (fabs(TRGT_H - width * scale_h) <= fabs(TRGT_W - height * scale_w))
		img->zoom = scale_h;
	else
		img->zoom = scale_w;
}

t_data	*init_window(t_data *img)
{
	img->width = 1920;
	img->height = 1080;

	img->mlx = mlx_init(img->width, img->height, "FDF", 1);
	if (!img->mlx)
		return(NULL);
	img->img = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->img)
		return (NULL);
	img->mode = ISO;

	return (img);
}

t_point	rotation_iso(t_point point)
{
	t_point	temp;

	//Rz rotation for 45 i think the ANGL_X and ANGL_Y application is not right as it is
	float angle_45;
	float angle_30;

	angle_45 = 45 * (PI / 180);
	angle_30 = 30 * (PI / 180);
	temp.x = ANGL_X * point.x * cos(angle_45) - ANGL_Y * point.y * sin(angle_45);
	temp.y = ANGL_Y * point.y * cos(angle_45) + ANGL_X * point.x * sin(angle_45);

	point.x = temp.x;
 	point.y = temp.y;

	//Rx rotation for 30
	temp.y = ANGL_Y * point.y * cos(angle_30) - DEPTH * point.z * sin(angle_30);
	temp.z = DEPTH * point.z * cos(angle_30) + ANGL_Y * point.y * sin(angle_30);

	point.y = temp.y;
	point.z = temp.z;

	return (point);
}

void	draw_map(t_list **map, t_data *img)
{
	t_list	*ptr;
	int		x;
	int		y;

	ptr = *map;
	while (ptr)
	{
		x = 0;
		while (x < ptr->width)
		{
			y = ptr->curr_pos;
			if (x < (*map)->width - 1)
				drw_line(new_p(x, y, ptr, img), new_p(x + 1, y, ptr, img), img);
			if (y < (*map)->height - 1)
				drw_line(new_p(x, y, ptr, img), new_p(x, y + 1, ptr->next, img), img);
			x ++;
		}
		ptr = ptr->next;
	}
}

t_point	new_p(int x, int y, t_list *map, t_data *img)
{
	t_point	point;

	point.x = x;
	point.y = y;
	point.z = map->int_array[x];
	//if (map->int_array[x][1] != MAGENTA)
	point.color = map->color_array[x];

	point = apply_camera(point, img);
	//add rotation option - rotate around x or y or z
	if (img->mode == ISO)
		point = rotation_iso(point);
	//somehow axel didnt shift the z and it was fine, let's try like this


	//it should be shifting to the center by calculating the line length (using zoom)
	// point.x += map->width * img->zoom * map->width / 2;
	// point.y += map->height * img->zoom * map->height / 2;

	point.x += img->width / 2.5;
	point.y += img->height / 4.8;
	return (point);
}

t_point	apply_camera(t_point point, t_data *img)
{
	//some problem with int
	point.x *= img->zoom;
	point.y *= img->zoom;
	point.z *= img->zoom * DEPTH;
	return (point);
}


void	drw_line(t_point point, t_point dest, t_data *img)
{
	t_point	delta;
	t_point	sign;
	int		error;
	int		error_2;

	delta.x = ft_abs(dest.x - point.x);
	delta.y = ft_abs(dest.y - point.y);
	vector_find_sign(point, dest, &sign);
	error = delta.x - delta.y;

	while (point.x != dest.x || point.y != dest.y)
	{
		if (point.x < img->width && point.y < img->height && point.x >= 0 && point.y >= 0)
			mlx_put_pixel(img->img, point.x, point.y, point.color);
		error_2 = error * 2;
		if (error_2 > -delta.y)
			error -= delta.y;
		if (error_2 > -delta.y)
			point.x += sign.x; //increment + or -1
		if (error_2 < delta.x)
			error += delta.x;
		if (error_2 < delta.x)
			point.y += sign.y;  //increment + or -1
	}
	//if (point.x < img->width && point.y < img->height && point.x >= 0 && point.y >= 0)
	//	mlx_put_pixel(img->img, point.x, point.y, point.color);
	//do i need to draw another pixel?
}

int	ft_abs(int result)
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
}

static void ft_hook(void* param)
{

	const mlx_t* mlx = param;

	(void)mlx;

	//ft_printf("WIDTH: %d | HEIGHT: %d\n", mlx->width, mlx->height);
	//ft_printf("hooking....");
}



// void	draw_line(t_point point, t_point dest, t_data *img)
// {

// 	double dx = dest.x - point.x;
//     double dy = dest.y - point.y;
//     double steps = fmax(fabs(dx), fabs(dy));

//     double x_inc = dx / steps;
//     double y_inc = dy / steps;

// 	uint32_t x_int;
// 	uint32_t y_int;
// 	int i = 0;
// 	while (i < steps)
// 	{
// 		x_int = (uint32_t)round(point.x);
// 		y_int = (uint32_t)round(point.y);

// 		if (point.x < img->width && point.y < img->height && point.x && point.y)
// 			mlx_put_pixel(img->img, point.x, point.y, point.color);

// 		point.x += x_inc;
// 		point.y += y_inc;
// 		i ++;
// 	}

// }
