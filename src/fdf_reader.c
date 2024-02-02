/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:51:54 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/01 16:58:47 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

t_list	*fdf_reader(int fd)
{
	t_list	*line_list;

	line_list = ft_lstnew(NULL);
	if (!line_list)
		return (NULL);
	if (!gnl_to_list_and_check(&line_list, fd))
	{
		ft_lstclear(&line_list, free);
		return (NULL);
	}
	if (!split_lines_to_strings(&line_list))
		return (NULL);
	if (!strings_to_numbers(&line_list))
		return (NULL);
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

t_list	**split_lines_to_strings(t_list **line_list)
{
	t_list	*ptr;

	ptr = *line_list;
	while (ptr)
	{
		ptr->cells = ft_split(ptr->line, ' ');
		if (!ptr->cells)
		{
			ft_lstclear(line_list, free);
			return (NULL);
		}
		free (ptr->line);
		ptr->line = NULL;
		ptr = ptr->next;
	}
	return (line_list);
}

t_list	**strings_to_numbers(t_list **line_list)
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
		if (!ptr->int_array || !ptr->color_array)
		{
			ft_lstclear(line_list, free);
			return (NULL);
		}
		i = -1;
		while (ptr->cells[++i])
			put_colors_in_list(line_list, ptr, i);
		ptr = ptr->next;
	}
	return (line_list);
}
