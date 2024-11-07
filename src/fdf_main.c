/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/11/07 19:54:23 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

int	main(int ac, char **av)
{
	t_list	*map;
	int		fd;
	int		name_len;

	if (ac == 1 || ac == 2)
	{
		if (ac == 1)
			av[1] = "test_maps/100-6.fdf";
		fd = open(av[1], O_RDONLY);
		if (fd < 0)
			return (1);
		name_len = ft_strlen(av[1]);
		if (name_len > 3 && ft_strncmp(av[1] + name_len - 4, ".fdf", 5) != 0)
			return (2);
		map = fdf_reader(fd);
		close(fd);
		if (fd < 0 || !map)
			return (3);
		init_and_draw(&map);
		ft_lstclear(&map, free);
	}
	else
		return (4);
	return (EXIT_SUCCESS);
}

void	init_and_draw(t_list **map)
{
	t_data	*img;

	img = malloc(sizeof(t_data));
	if (!img)
	{
		free (img);
		return ;
	}
	zoom_calc(*map, img);
	img = init_window(img, *map);
	if (!img || (mlx_image_to_window(img->mlx, img->instance, 0, 0) < 0))
	{
		if (img)
			mlx_terminate(img->mlx);
		free (img);
		return ;
	}
	img->map = map;
	draw_map(img->map, img);
	mlx_loop_hook(img->mlx, &ft_hook_hub, img);
	mlx_scroll_hook(img->mlx, &my_scrollhook, img);
	mlx_loop(img->mlx);
	mlx_terminate(img->mlx);
	free (img);
}

t_data	*init_window_params(t_data *img)
{
	img->width = TRGT_W;
	img->height = TRGT_H;
	img->shift_x = -100;
	img->shift_y = 0;
	img->shift_z = 0.2;
	img->angle_x = 0;
	img->angle_y = 0;
	img->angle_z = 0;
	img->malloc_check = 0;
	img->height_col = MAGENTA;
	img->mlx = mlx_init(img->width, img->height, "FDF", 0);
	if (!img->mlx)
		return (NULL);
	img->backg = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->backg)
		return (NULL);
	img->instance = mlx_new_image(img->mlx, img->width, img->height);
	if (!img->instance)
		return (NULL);
	return (img);
}

t_data	*init_window(t_data *img, t_list *map)
{
	if (!init_window_params(img))
	{
		mlx_terminate(img->mlx);
		return (NULL);
	}
	if (!put_background(img))
	{
		mlx_terminate(img->mlx);
		return (NULL);
	}
	shift_outofbounds(map, img);
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
	mlx_put_string(img->mlx, "ISOMETRIC/TOP VIEW:  r/1", 6, 1);
	mlx_put_string(img->mlx, "ROTATE:              y/h i/k j/l", 6, 19);
	mlx_put_string(img->mlx, "ZOOM:                scroll | +/-", 6, 37);
	mlx_put_string(img->mlx, "CHANGE DEPTH:        z/x", 6, 55);
	mlx_put_string(img->mlx, "TRANSLATE:           wasd", 6, 73);
	mlx_put_string(img->mlx, "Z COLOR:             m/n", 6, 91);
	mlx_put_string(img->mlx, "TOP VIEW:            1", 6, 109);
	return (img);
}
