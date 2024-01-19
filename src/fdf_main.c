/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/19 17:26:41 by klukiano         ###   ########.fr       */
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

// #define WIDTH 1024
// #define HEIGHT 768

t_list	*fdf_reader(int fd);
int		check_and_del_newline(char *l);
int		is_cell_colored(char *str);
int		is_valid_hex(char *str);
static void ft_hook(void* param);
void	draw_and_move(t_list **map);
void	draw_line(mlx_image_t *img, double x, double y, double x_dest, double y_dest);
void	rotation_iso(double *x, double *y);

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
		draw_and_move(&map);
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

		// 	////very strange thing, printf breaks if i mix my printf with the system one
		// 	map = map->next;
		// }
	}
	ft_lstclear(&map, free);
	return (EXIT_SUCCESS);
}

void	draw_and_move(t_list **map)
{
	mlx_t *mlx;
	t_list *ptr;

	//mlx_set_setting(MLX_STRETCH_IMAGE, true);

	double		scale;

	// turn into proper calculation
	if ((*map)->width < 200 || (*map)->height < 200)
		scale = 25;
	else
		scale = 2;

	double		shift;
	double x_coord = 0;
	double y_coord = 0;
	double	x_dest = 0;
	double	y_dest = 0;

	shift = 100;
	// ptr = *map;
	//
	// while (ptr->next)
	// {
	// 	i ++;
	// 	ptr = ptr->next;
	// }
	// x_coord = j * width_line;
	// y_coord = i * height_line;
	// rotation_iso(&x_coord, &y_coord);

	// x_dest = (j + 1) * width_line;
	// y_dest = (i + 1) * height_line;
	// rotation_iso(&x_dest, &y_dest);
	// ft_printf("the x is %d, the y is %d, the xdest is %d, the ydest is %d\n", x_coord, y_coord, x_dest, y_dest);
	// return ;

	mlx = mlx_init(((*map)->width * scale + shift + 100) , ((*map)->height * scale + shift + 100), "test_name", 1);
	if (!mlx)
	{
		ft_lstclear(map, free);
		//ft_printf(stderr, "%s", mlx_strerror(mlx_errno));
		exit(EXIT_FAILURE);
	}


	mlx_image_t* img = mlx_new_image(mlx, (*map)->width * scale + shift + 100, (*map)->height * scale + shift + 100);

	//int		shift_amount = 5 * scale;
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
	{
		ft_lstclear(map, free);
		//ft_printf(stderr, "%s", mlx_strerror(mlx_errno));
		exit(EXIT_FAILURE);
	}

	double width_line = img->width / (*map)->width;
	double height_line = img->height / (*map)->height;

	int x = 0;
	int y = 0;
	int f = 0;
	int	j = 0;
	int i = 0;
	ptr = *map;
	//shift the picture to the right by the max amount so that aftter x - y in rotation iso we alwyas have a positive number
	while (ptr)
	{
		//x_coord = 0 ;
		j = 0;
		while (j < (*map)->width)
		{
			x_coord = j * width_line;
			y_coord = i * height_line;
			rotation_iso(&x_coord, &y_coord);

			x_dest = (j + 1) * width_line;
			y_dest = (i + 1) * height_line;
			rotation_iso(&x_dest, &y_dest);
			if (j + 1 < (*map)->width)
			{
				draw_line (img, x_coord + shift, y_coord + shift, x_dest + shift, y_coord + shift);
			}
			//if (ptr->next)
			//	draw_line (img, x_coord + shift, y_coord + shift, x_coord + shift, y_dest + shift);
			j ++;
		}
		ptr = ptr->next;
		i ++;
	}

	// mlx_put_pixel(img, 150, 150, 0xAF0000FF);
	// mlx_put_pixel(img, 300, 300, 0xFF0000FF);

	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!

	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	ft_printf("\n");
}

void	rotation_iso(double *x, double *y)
{
	*x = (*x - *y) * cos( 45 * (PI / 180.0));
	*y = (*x + *y) * sin( 30 * (PI / 180.0)) * -1;
	return ;
}

void	draw_line(mlx_image_t *img, double x, double y, double x_dest, double y_dest)
{

	//consider implementing Bersenham or even Wu

	double dx = x_dest - x;
	double dy = y_dest - y;
	double steps = fmax(fabs(dx), fabs(dy)); // use fabs for floating-point absolute value

	double x_inc ;
	double y_inc;
	if (steps)
	{
		x_inc = dx / steps;
		y_inc = dy / steps;
	}
	else
	{
		x_inc = 0;
		y_inc = 0;
	}
	uint32_t x_int;
	uint32_t y_int;
	int i = 0;
	while (i < steps)
	{
		x_int = (uint32_t)round(x);
		y_int = (uint32_t)round(y);

		mlx_put_pixel(img, x_int, y_int, 0xFFFFFF99); // use the rounded and casted values here
		x += x_inc;
		y += y_inc;
		i ++;
	}

	//find out the smallest step??
	//
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
	//t_list	*map;
	t_list	*line_list;
	//char	*line;
	//char	*temp;
	//char	*big_line;
	//int		is_error;
	//char	**split_str;
	int		i;
	int 	j;
	t_list	*ptr;
	//int		bytes;

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

	// count the amount of cells and if it has ',' then add + 1 each time
	// ptr = line_list;
	// i = 0;
	// while (ptr)
	// {
	// 	j = 0;
	// 	while (ptr->cells[j])
	// 	{
	// 		i ++;
	// 		j ++;
	// 	}
	// 	ptr = ptr->next;
	// }
	// i = i * 2;
	// ft_printf("The amount of cells times two is %d\n", i);
	// convert the cells to numbers

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
			else
				ptr->int_array[i][1] = -1;
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
	i = ptr->width;
	amount = 0;
	while (ptr)
	{
		if (i != ptr->width)
		{
			ft_lstclear(&line_list, free);
			ft_printf("THE WIDTH IS DIFFERENT\n");
			return (NULL);
		}
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
