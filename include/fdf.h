/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 16:18:49 by klukiano          #+#    #+#             */
/*   Updated: 2024/11/06 19:46:33 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define PI 3.1415
# define TRGT_W 1920
# define TRGT_H 1080
# define MAGENTA 0xFF00FFEE
# define PURPLE 0xB800FFEE
# define WHITE 0xFFFFFFEE
# define YELLOW 0xFFFF00EE
# define YEL_WHITE 0xFFFFB6EE

# include <math.h>
# include <fcntl.h>
# include "../lib/libft/libft.h"
// # include "../lib/ft_printf/ft_printf.h"
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"
//# include "../lib/MLX42/include/MLX42/MLX42_Int.h"

typedef struct s_point
{
	int				x;
	int				y;
	float			z;
	unsigned int	color;
	void			*content;
}					t_point;

typedef struct s_data
{
	mlx_t			*mlx;
	mlx_image_t		*instance;
	mlx_image_t		*backg;
	int32_t			width;
	int32_t			height;
	float			zoom;
	int32_t			shift_x;
	int32_t			shift_y;
	int				angle_z;
	int				angle_x;
	int				angle_y;
	float			shift_z;
	uint32_t		height_col;
	int				malloc_check;
	t_list			**map;
	mlx_key_data_t	keydata;
}					t_data;

int				check_and_del_newline(char *l);
int				is_cell_colored(char *str);
int				is_valid_hex(char *str);

//	fdf_hook_control_hub_bonus.c
void			ft_hook_hub(void *param);
void			check_changes_to_img(t_data *prev, t_data *img);
t_data			*redraw_image(t_data *img);

//	fdf_hook_controls_bonus.c
void			ft_hook_movement(t_data *img);
void			hook_rotation(t_data *img);
void			hook_zoom_buttons(t_data *img);
void			my_scrollhook(double xdelta, double ydelta, void *param);
void			hook_reset_image(t_data *img);

//	fdf_colors_to_list.c
void			put_colors_in_list(t_list **line_list, t_list *ptr, int i);
void			check_width_set_height(t_list **line_list);
int				is_cell_colored(char *str);
int				is_valid_hex(char *str);

//	fdf_draw.c
void			draw_map(t_list **map, t_data *img);
void			drw_line(t_point point, t_point dest, t_data *img);
void			delta_sign(t_point *dlt, t_point p, t_point dst, t_point *sign);
void			put_pixel(t_data *img, t_point *point, float *c_step);

//	fdf_get_colors.c
int				get_r(unsigned int rgba);
int				get_g(unsigned int rgba);
int				get_b(unsigned int rgba);
int				get_a(unsigned int rgba);
unsigned int	add_full_alpha(unsigned int color);

//	fdf_gradient.c
float			*cstep(float *cstep, t_point *p, t_point *dst, t_point *dlt);
void			gradient_step(t_point *point, float *colorstep);

//	fdf_main.c
t_list			*fdf_reader(int fd);
void			init_and_draw(t_list **map);
t_data			*init_window(t_data *img, t_list *map);
t_data			*init_window_params(t_data *img);
t_data			*put_background(t_data *img);

//	fdf_point.c
t_point			new_p(int x, int y, t_list *map, t_data *img);
unsigned int	height_to_color(t_point point, t_data *img);
t_point			rotation_func(t_point point, t_data *img);

//	fdf_reader.c
t_list			*fdf_reader(int fd);
t_list			*gnl_to_list_and_check(t_list **line_list, int fd);
int				check_and_del_newline(char *l);
t_list			**split_lines_to_strings(t_list **line_list);
t_list			**strings_to_numbers(t_list **line_list);

//	ft_atoi_b.c
long			ft_atoi_b(const char *str, int str_base);

// helper_functions.c
int				free_n_0(int *int_arr, char **str_arr, unsigned int *color_arr);
float			zoom_calc(t_list *map, t_data *img);
int				ft_abs(int result);
void			shift_outofbounds(t_list *map, t_data *img);
void			p_bounds(t_point *p, t_point index, t_list *ptr, t_data *img);

#endif
