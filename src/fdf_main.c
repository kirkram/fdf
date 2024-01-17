/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/17 18:21:01 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../lib/libft/libft.h"
#include "../lib/get_next_line/get_next_line.h"
#include "../lib/MLX42/include/MLX42/MLX42.h"
#include "../include/fdf.h"
#include <fcntl.h>

t_list	*fdf_reader(int fd, char **av);
int		check_and_del_newline(char *l);

int	main(int ac, char **av)
{
	t_list	*map;
	int		fd;

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
			return (1);
		map = fdf_reader(fd, av);
		if (!map)
			return (1);
		int i;
		while (map)
		{
			i = 0;
			while (i < map->amount)
			{
				printf("%i, %i", map->int_array[i][0], map->int_array[i][1]);
				i ++;
			}
			map = map->next;
		}
		printf("success?\n");
	}
	return (0);
}

t_list	*fdf_reader(int fd, char **av)
{
	//t_list	*map;
	t_list	*line_list;
	char	*line;
	char	*temp;
	char	*big_line;
	int		is_error;
	char	**split_str;
	int		i;
	int 	j;
	t_list	*ptr;
	int		bytes;

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
			ft_lstclear(line_list, free);
			return (NULL);
		}
		free (ptr->line);
		ptr->line = NULL;
		ptr = ptr->next;
	}

	// count the amount of cells and if it has ',' then add + 1 each time
	ptr = line_list;
	i = 0;
	while (ptr)
	{
		j = 0;
		while (ptr->cells[j])
		{
			i ++;
			j ++;
		}
		ptr = ptr->next;
	}
	i = i * 2;
	printf("The amount of cells is %d\n", i);
	// convert the cells to numbers

	ptr = line_list;
	i = 0;
	int amount;
	while (ptr)
	{
		amount = 0;
		while (ptr->cells[amount])
			amount ++;
		ptr->amount = amount;
		ptr->int_array = malloc(amount * sizeof(int *));
		i = 0;
		while (ptr->cells[i])
		{
			j = 1;
			ptr->int_array[i] = malloc(2 * sizeof(int));
			if (is_cell_colored(ptr->cells[i]))
			{
				while (ptr->cells[i][j] != 'x')
					j ++;
				//check if works if there is something after the hex number or if its longer
				if (ptr->cells[i][j + 7] != ' ' || ptr->cells[i][j + 7] != '\0')
				{
					ft_lstclear(line_list, free);
					return (NULL);
				}
				ptr->int_array[i][1] = ft_atoi_base(ptr->cells[i] + j, 16);
			}
			ptr->int_array = ft_atoi(ptr->cells[i][0]);
			//worth a whole function this check?
			if (ptr->int_array[i][0] == 0 && ptr->cells[i][0] != '0')
			{
				ft_lstclear(line_list, free);
				return (NULL);
			}
			else if (ptr->int_array[i][0] == -1 && ptr->cells[i][0] != '-')
			{
				ft_lstclear(line_list, free);
				return (NULL);
			}
			i ++;
		}
		ptr = ptr->next;
	}

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

	while (str[i])
	{
		if (str[i] == ',')
			return (1);
		i ++;
	}
	return (0);
}
