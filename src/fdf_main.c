/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/17 13:12:02 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../lib/libft/libft.h"
#include "../lib/get_next_line/get_next_line.h"
#include "../lib/MLX42/include/MLX42/MLX42.h"
#include <fcntl.h>

t_list	*fdf_reader(int fd, char **av);

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
		int i = 0;
		while (map)
		{
			printf("%s\n", map->line);
			map = map->next;
			i ++;
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
		ft_lstadd_back(&line_list, ft_lstnew(NULL));
		ptr = ptr->next;
	}


	return (line_list);
}

// int	check_lines(char **split_str)
// {
// 	int	i;

// 	i = 0;
// 	while (split_str[i])
// 	{

// 		i ++;
// 	}
// 	return (1);
// }
