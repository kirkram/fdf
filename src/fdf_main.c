/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/29 17:58:11 by klukiano         ###   ########.fr       */
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
void 	ft_hook_hub(void* param);
t_list	*ft_pointnew(void *content);
void	draw_and_move_00(t_list **map);

void	init_and_draw(t_list **map);
void	draw_map(t_list **map, t_data *img);
t_point	new_p(int x, int y, t_list *map, t_data *img);
void	drw_line(t_point point, t_point dest, t_data *img);
int	ft_abs(int result);
void	vector_find_sign(t_point point, t_point dest, t_point *sign);
t_point	apply_camera(t_point point, t_data *img);
float		zoom_calc(t_list *map, t_data *img);
t_data	*init_window(t_data *img);
t_point	rotation_func(t_point point, t_data *img);
unsigned int	gradient_step(t_point point, float *colorstep);
unsigned int height_to_color(t_point point);
void	my_scrollhook(double xdelta, double ydelta, void* param);
void my_keyhook(mlx_key_data_t keydata, void* param);
t_data	*redraw_image(t_data *img);

void	draw_line(t_point point, t_point dest, t_data *img);
t_data	*put_background(t_data *img);

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
		close(fd);
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

void ft_hook_hub(void* param)
{
	t_data	*img;

	img = param;
	if (mlx_is_key_down(img->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(img->mlx);

	else if (mlx_is_key_down(img->mlx, MLX_KEY_A))
	{
		img->shift_x -= 10;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_D))
	{
		img->shift_x += 10;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_W))
	{
		img->shift_y -= 10;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_S))
	{
		img->shift_y += 10;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_Z))
	{
		img->shift_z -= 0.01;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_X))
	{
		img->shift_z += 0.01;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_J))
	{
		img->angle_z += 1;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_L))
	{
		img->angle_z -= 1;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_I))
	{
		img->angle_x += 1;
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_K))
	{
		img->angle_x -= 1;
		redraw_image(img);
	}
	// else if (mlx_is_key_down(img->mlx, MLX_KEY_H))
	// {
	// 	while ()
	// 	{
	// 		img->angle_rz += 1;
	// 		draw_map(img->map, img);
	// 	}
	// }
	else if (mlx_is_key_down(img->mlx, MLX_KEY_R))
	{
		img->shift_x = 0;
		img->shift_y = 0;
		img->shift_z = 0.1;
		img->angle_z = 0;
		img->zoom = zoom_calc(*img->map, img);
		redraw_image(img);
	}
	else if (mlx_is_key_down(img->mlx, MLX_KEY_KP_ADD))
	{

		if (img->width < 100)
			img->zoom += 1;
		else
			img->zoom += 0.5;

		redraw_image(img);
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
		redraw_image(img);
	}
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_data	*img;
	(void)keydata;
	img = param;

}

t_data	*redraw_image(t_data *img)
{
	mlx_delete_image(img->mlx, img->img);
	img->img = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->img)
	{
		mlx_close_window(img->mlx);
		return (NULL);
	}
	mlx_image_to_window(img->mlx, img->img, 0, 0);
	draw_map(img->map, img);

	return (img);
}

void my_scrollhook(double xdelta, double ydelta, void* param)
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
	img->mode = ISO;
	img->angle_x = 0;
	img->angle_z = 0;

	img->mlx = mlx_init(img->width, img->height, "FDF", 0);
	if (!img->mlx)
		return(NULL);
	img->backg = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->backg)
		return (NULL);
	img->img = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->img || !img->backg)
		return (NULL);
	img->mode = ISO;

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
	return (img);
}

void	draw_map(t_list **map, t_data *img)
{
	t_list	*ptr;
	int		x;
	int		y;

	ptr = *map;
	//put_background(img);

	y = 0;

	while (ptr)
	{
		x = 0;
		while (x < ptr->width)
		{
			if (x < (*map)->width - 1)
				drw_line(new_p(x, y, ptr, img), new_p(x + 1, y, ptr, img), img);
			if (y < (*map)->height - 1)
				drw_line(new_p(x, y, ptr, img), new_p(x, y + 1, ptr->next, img), img);
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
		point.color = height_to_color(point);
	point = apply_camera(point, img);
	point = rotation_func(point, img);
	point.x += img->shift_x;
	point.y += img->shift_y;
	point.x += img->width / 2;
	point.y += img->height / 5;
	return (point);
}

unsigned int height_to_color(t_point point)
{
	if (point.z > 0)
		point.color = MAGENTA;
	if (point.z < 0)
		point.color = PURPLE;
	return (point.color);
}

t_point	apply_camera(t_point point, t_data *img)
{
	point.x *= img->zoom;
	point.y *= img->zoom;
	point.z *= img->zoom;
	return (point);
}

t_point	rotation_func(t_point point, t_data *img)
{
	t_point	temp;

	//Rz rotation for 45 i think the ANGL_X and ANGL_Y application is not right as it is
	float angle_45;
	float angle_30;
	int	angle_x;
	int angle_z;
	(void)img;

	//here we'd change the 45a and 30 to a different angle according to user input
	//the default values will be depending on the MODE value
	//45 = 0.78539816339
	//30 = 0.52359877559


	angle_x = img->angle_x;
	angle_z = img->angle_z;

	angle_45 = (45 + angle_z) * (PI / 180);
	angle_30 = (30 + angle_x) * (PI / 180);

	//This is Rz
	temp.x = point.x * cos(angle_45) - point.y * sin(angle_45);
	temp.y = point.y * cos(angle_45) + point.x * sin(angle_45);

	point.x = temp.x;
 	point.y = temp.y;

	//This is Rx
	temp.y = point.y * cos(angle_30) - point.z * sin(angle_30);
	temp.z = point.z * cos(angle_30) + point.y * sin(angle_30);

	point.y = temp.y;
	point.z = temp.z;

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

	//можно ли его использовать?
	//float line_len_hypotenuse = sqrt(delta.x * delta.x + delta.y * delta.y);


	float draw_steps = delta.y;
	if (delta.x > delta.y)
		draw_steps = delta.x;
	if (draw_steps == 0)
		draw_steps ++;

	draw_steps = sqrt(delta.x * delta.x + delta.y * delta.y);

	float colorstep[4];
	colorstep[0] = (get_r(dest.color) - get_r(point.color)) / draw_steps;
	colorstep[1] = (get_g(dest.color) - get_g(point.color)) / draw_steps;
	colorstep[2] = (get_b(dest.color) - get_b(point.color)) / draw_steps;


	while (point.x != dest.x || point.y != dest.y)
	{
		if (point.x < img->width && point.y < img->height && point.x >= 0 && point.y >= 0)
		{
			mlx_put_pixel(img->img, point.x, point.y, point.color);
			point.color = gradient_step(point, colorstep);
		}
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
unsigned int	gradient_step(t_point point, float *colorstep)
{
	int	newcolor[4];
	unsigned int color;
	int						i;

	newcolor[0] = get_r(point.color);
	newcolor[1] = get_g(point.color);
	newcolor[2] = get_b(point.color);
	newcolor[3] = get_a(point.color);

	i = 0;
	while (i < 3)
	{
		newcolor[i] = (int)round(newcolor[i] + colorstep[i]);
		i ++;
	}
	i = 0;
	while (i < 3)
	{
		if (newcolor[i] < 0)
			newcolor[i] = 0;
		else if (newcolor[i] > 255)
			newcolor[i] = 255;
		i ++;
	}
	color = ((unsigned int)newcolor[0] << 24) | ((unsigned int)newcolor[1] << 16) | ((unsigned int)newcolor[2] << 8) | (unsigned int)newcolor[3];;
	return (color);
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
