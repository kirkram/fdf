/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/23 19:37:09 by klukiano         ###   ########.fr       */
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

#define PI 3.14159265358979323846

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
t_data	*init_window(t_data *img, t_list **map);
t_point	rotation_iso(t_point point);

int	main(int ac, char **av)
{
	t_list	*map;
	int		fd;
	int		i;

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
			return (1);
		map = fdf_reader(fd);
		if (!map)
			return (1);
		init_and_draw(&map);
		// while (map)
		// {
		// 	i = 0;
		// 	while (i < map->width)
		// 	{
		// 		if (map->int_array[i][1] != -1)
		// 			ft_printf("%i, %x ", map->int_array[i][0], map->int_array[i][1]);
		// 		else
		// 			ft_printf("%i [ ] ", map->int_array[i][0]);
		// 		i ++;
		// 	}
		// 	ft_printf("\n");
	}
	ft_lstclear(&map, free);
	return (EXIT_SUCCESS);
}

void	init_and_draw(t_list **map)
{
	t_data	*img;

	img = malloc(sizeof(t_data));
	zoom_calc(*map, img);
	img = init_window(img, map);
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

void	draw_map(t_list **map, t_data *img)
{
	t_point	dest;
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
			if (x < ptr->width - 1)
				drw_line(new_p(x, y, ptr, img), new_p(x + 1, y, ptr, img), img);
			if (y < ptr->height - 1)
				drw_line(new_p(x, y, ptr, img), new_p(x, y + 1, ptr, img), img);
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
	point.z = map->int_array[x][0];
	point.color = map->int_array[x][1];
	point = apply_camera(point, img);
	//add rotation option - rotate around x or y or z
	if (img->mode == ISO)
		point = rotation_iso(point);
	//somehow axel didnt shift the z and it was fine, let's try like this
	point.x += img->width / 4;
	point.y += img->height / 4;
	return (point);
}

void	drw_line(t_point point, t_point dest, t_data *img)
{
	//implement bresenham from the chat convo
	t_point	delta;
	t_point	sign;
	int		error;
	int		error_2;

	delta.x = ft_abs(dest.x - point.x);
	delta.y = ft_abs(dest.y - point.y);
	vector_find_sign(point, dest, &sign);
	error = delta.x - delta.y;
	//where is the check for the bounds of the image?
	while (point.x != dest.x && point.y != dest.y)
	{
		if (point.x < img->width && point.y < img->height && point.x && point.y)
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

t_point	apply_camera(t_point point, t_data *img)
{
	//some problem with int
	point.x *= img->zoom;
	point.y *= img->zoom;
	point.z *= img->zoom;
	return (point);
}

void	zoom_calc(t_list *map, t_data *img)
{
	double	scale_w;
	double	scale_h;
	int		width;
	int		height;

	width = map->width;
	height = map->height;
	scale_w = TRGT_W / width;
	scale_h = TRGT_H / height;
	if (fabs(TRGT_H - width * scale_h) <= fabs(TRGT_W - height * scale_w))
		img->zoom = scale_h;
	else
		img->zoom = scale_w;
}

t_data	*init_window(t_data *img, t_list **map)
{
	img->width = (uint32_t)round((*map)->width * img->zoom);
	img->height = (uint32_t)round((*map)->height * img->zoom);

	img->mlx = mlx_init(img->width, img->height, "FDF", 1);
	if (!img->mlx)
		return(NULL);
	img->img = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->img)
		return (NULL);
	// img->line_len_w = img->width / (*map)->width;
	// img->line_len_h = img->height / (*map)->height;
	img->mode = ISO;

	return (img);
}

t_point	rotation_iso(t_point point)
{
	//radian of angle 45 is 45 * (PI / 180.0) = 0.78539816339
	//radian of anglle 30 is 30 * (PI / 180.0) = 0.52359877559
	//left hand rotation is cos sin for x -sin cos for y
	point.x = point.x * cos(0.78539816339) + point.y * sin(0.78539816339);
	point.y = point.y * cos(0.78539816339) - point.x * sin(0.78539816339);
	//then apply Rx 30
	point.y = point.y * cos(0.52359877559) + point.z * sin(0.52359877559);
	point.z = point.z * cos(0.52359877559) - point.y * sin(0.52359877559);

	return (point);
}

static void ft_hook(void* param)
{

	const mlx_t* mlx = param;

	(void)mlx;

	//ft_printf("WIDTH: %d | HEIGHT: %d\n", mlx->width, mlx->height);
	ft_printf("hooking....");
}


t_list	*fdf_reader(int fd)
{
	t_list	*line_list;
	int		i;
	int 	j;
	t_list	*ptr;

	i = 0;
	line_list = ft_lstnew(NULL);
	if (!line_list)
		return (NULL);
	ptr = line_list;
	while (1)
	{
		ptr->line = get_next_line(fd);
		//doesnt care about failing gnl malloc but whatever
		if (!ptr->line && read(fd, NULL, 0) < 0)
		{
			ft_lstclear(&line_list, free);
			return (NULL);
		}
		else if (!ptr->line)
		{
			ptr = line_list;
			while (ptr->next->line)
				ptr = ptr->next;;
			free (ptr->next);
			ptr->next = NULL;
			break;
		}
		if (!check_and_del_newline(ptr->line))
		{
			ft_lstclear(&line_list, free);
			return (NULL);
		}
		ft_lstadd_back(&line_list, ft_lstnew(NULL));
		ptr = ptr->next;
	}

	//convert to individ strings and free the line
	ptr = line_list;
	while (ptr)
	{
		ptr->cells = ft_split(ptr->line, ' ');
		if (!ptr->cells)
		{
			ft_lstclear(&line_list, free);
			return (NULL);
		}
		free (ptr->line);
		ptr->line = NULL;
		ptr = ptr->next;
	}


	ptr = line_list;
	i = 0;
	int amount;
	while (ptr)
	{
		amount = 0;
		while (ptr->cells[amount])
			amount ++;
		ptr->width = amount;
		ptr->int_array = malloc(amount * sizeof(int *));
		i = 0;
		while (ptr->cells[i])
		{
			j = 1;
			//is it being freed properly by lstclear?
			ptr->int_array[i] = malloc(2 * sizeof(int));
			if (is_cell_colored(ptr->cells[i]))
			{
				while (ptr->cells[i][j] != 'x')
					j ++;
				//check if works if there is something after the hex number or if its longer

				//IT WILL NOT WORK IF THE HEX NUMBER IS SHORT!!!!!!!! think again
				//if (ptr->cells[i][j + 7] != ' ' || ptr->cells[i][j + 7] != '\0')
				if (!is_valid_hex(ptr->cells[i] + j + 1))
				{
					ft_lstclear(&line_list, free);
					return (NULL);
				}
				ptr->int_array[i][1] = ft_atoi_base(ptr->cells[i] + j + 1, 16);
			}
			//THE COLOR IS HERE
			else
				ptr->int_array[i][1] = 0xFFFFFF;
			ptr->int_array[i][0] = ft_atoi(ptr->cells[i]);
			//worth a whole function this check?
			if (ptr->int_array[i][0] == 0 && ptr->cells[i][0] != '0')
			{
				ft_lstclear(&line_list, free);
				return (NULL);
			}
			else if (ptr->int_array[i][0] == -1 && ptr->cells[i][0] != '-')
			{
				ft_lstclear(&line_list, free);
				return (NULL);
			}
			i ++;
		}
		ptr = ptr->next;
	}

	ptr = line_list;
	int	first_line_width = ptr->width;
	amount = 0;
	ptr->curr_pos = -1;
	while (ptr)
	{
		if (first_line_width != ptr->width)
		{
			ft_lstclear(&line_list, free);
			ft_printf("THE WIDTH IS DIFFERENT\n");
			return (NULL);
		}
		ptr->curr_pos += 1;
		amount ++;
		ptr = ptr->next;
	}
	line_list->height = amount;
	return (line_list);

}

int	check_and_del_newline(char *l)
{
	int	i;

	i = 0;
	while (l[i])
	{
		if (!ft_isdigit(l[i]) && l[i] != ' ' && l[i] != '\n')
		{
			if (l[i] == '-' || l[i] == '+')
			{
				if (!ft_isdigit(l[i + 1]))
					return (0);
				else if ((l[i + 1] == '0') || (i > 0 && l[i - 1] != ' '))
					return (0);
			}
			else if (l[i] == ',' && l[i + 1] == '0' && l[i + 2] == 'x')
				while (l[i + 1] != ' ' && l[i + 1] != '\n' && l[i + 1] != '\0')
					i ++;
			else
				return (0);
		}
		i ++;
	}
	if (l[i - 1] == '\n')
		l[i - 1] = '\0';
	return (1);
}

int	is_cell_colored(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			return (1);
		i ++;
	}
	return (0);
}

int	is_valid_hex(char *str)
{
	int		i;
	char	*set;

	set = "0123456789ABCDEF";
	i = 0;
	while (str[i])
	{
		str[i] = ft_toupper(str[i]);
		i ++;
	}
	// ADD change to limit the number to FFFFFF;


	//it should work wo4thout cause split should ve deleted all extra spaces
	// i = -1;
	// while (str[i++])
	// 	if (str[i] == ' ')
	// 		str[i] = '\0';

	while (*str)
	{
		i = 0;
		while (set[i])
		{
			if (*str == set[i])
				break ;
			i ++;
		}
		if (set[i] == '\0')
			return (0);
		str ++;
	}
	return (1);
}
