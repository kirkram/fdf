/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:51:54 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/31 15:59:04 by klukiano         ###   ########.fr       */
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

int				check_and_del_newline(char *l);
int				is_cell_colored(char *str);
int				is_valid_hex(char *str);
long			ft_atoi_b(const char *str, int str_base);
unsigned int	add_full_alpha(unsigned int color);
t_list			*gnl_to_list_and_check(t_list **line_list, int fd);
void			split_lines_to_strings(t_list **line_list);
void			check_width_set_height(t_list **line_list);
void			strings_to_numbers(t_list **line_list);
void			put_colors_in_list(t_list **line_list, t_list *ptr, int i);

t_list	*fdf_reader(int fd)
{
	t_list	*line_list;

	line_list = ft_lstnew(NULL);
	if (!line_list)
		exit (1);
	if (!gnl_to_list_and_check(&line_list, fd))
	{
		ft_lstclear(&line_list, free);
		return (NULL);
	}
	split_lines_to_strings(&line_list);
	strings_to_numbers(&line_list);
	check_width_set_height(&line_list);
	return (line_list);
}

t_list	*gnl_to_list_and_check(t_list **line_list, int fd)
{
	t_list	*ptr;
	int		gnl_check;

	ptr = *line_list;
	while (1)
	{
		gnl_check = get_next_line(&ptr->line, fd);
		if (!ptr->line && gnl_check == -1)
			return (NULL);
		else if (!ptr->line)
		{
			ptr = *line_list;
			while (ptr->next->line)
				ptr = ptr->next;
			free (ptr->next);
			ptr->next = NULL;
			break ;
		}
		if (!check_and_del_newline(ptr->line))
			return (NULL);
		ft_lstadd_back(line_list, ft_lstnew(NULL));
		ptr = ptr->next;
	}
	return (ptr);
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

void	split_lines_to_strings(t_list **line_list)
{
	t_list	*ptr;

	ptr = *line_list;
	while (ptr)
	{
		ptr->cells = ft_split(ptr->line, ' ');
		if (!ptr->cells)
		{
			ft_lstclear(line_list, free);
			exit (1);
		}
		free (ptr->line);
		ptr->line = NULL;
		ptr = ptr->next;
	}
}

void	strings_to_numbers(t_list **line_list)
{
	t_list	*ptr;
	int		i;
	int		amount;

	ptr = *line_list;
	i = 0;
	while (ptr)
	{
		amount = 0;
		while (ptr->cells[amount])
			amount ++;
		ptr->width = amount;
		ptr->int_array = malloc(amount * sizeof(int));
		ptr->color_array = malloc(amount * sizeof(unsigned int));
		i = 0;
		while (ptr->cells[i])
		{
			put_colors_in_list(line_list, ptr, i);
			i ++;
		}
		ptr = ptr->next;
	}
}

void	put_colors_in_list(t_list **line_list, t_list *ptr, int i)
{
	int		j;

	j = 1;
	if (is_cell_colored(ptr->cells[i]))
	{
		while (ptr->cells[i][j] != 'x')
			j ++;
		if (!is_valid_hex(ptr->cells[i] + j + 1))
		{
			ft_lstclear(line_list, free);
			exit (1);
		}
		ptr->color_array[i] = ft_atoi_b(ptr->cells[i] + j + 1, 16);
		if (ptr->color_array[i] <= 0xFFFFFF)
			ptr->color_array[i] = add_full_alpha(ptr->color_array[i]);
	}
	else
		ptr->color_array[i] = WHITE;
	ptr->int_array[i] = ft_atoi(ptr->cells[i]);
}

void	check_width_set_height(t_list **line_list)
{
	t_list	*ptr;
	int		first_line_width;
	int		amount;

	ptr = *line_list;
	first_line_width = ptr->width;
	amount = 0;
	while (ptr)
	{
		if (first_line_width != ptr->width)
		{
			ft_lstclear(line_list, free);
			exit (1);
		}
		amount ++;
		ptr = ptr->next;
	}
	ptr = *line_list;
	while (ptr)
	{
		ptr->height = amount;
		ptr = ptr->next;
	}
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
	i = -1;
	while (str[++i])
		str[i] = ft_toupper(str[i]);
	if (i > 8)
		return (0);
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
