/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_colors_to_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:01:16 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/01 12:22:50 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

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
	int	i;

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
