/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 13:23:45 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/17 12:25:48 by klukiano         ###   ########.fr       */
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
		// while (map)
		// {
		// 	printf("%s\n", map->line);
		// 	map = map->next;
		// 	i ++;
		// }
		//printf("success?\n");
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
	t_list	*ptr_list;
	int		bytes;

	big_line = NULL;
	is_error = 0;
	big_line = malloc(10000000 * sizeof(char));
	if (!big_line)
		return (NULL);
	bytes = read(fd, big_line, 10000000);
	if (bytes < 0 || bytes == 10000000)
	{
		if (bytes == 10000000)
			perror("The file is too big\n");
		free (big_line);
		return (NULL);
	}
	big_line[bytes] = '\0';
	split_str = ft_split(big_line, '\n');
	free (big_line);
	if (!split_str)
		return (NULL);
	i = 0;
	line_list = ft_lstnew(NULL);
	if (!line_list)
		return (NULL);
	ptr_list = line_list;
	while (split_str[i])
	{
		ptr_list->line = ft_strdup(split_str[i]);
		if (!ptr_list->line)
		{
			ft_lstclear(&line_list, free);
			free_n_0(NULL, split_str);
			return (NULL);
		}
		i ++;
		if (split_str[i])
			ft_lstadd_back(&line_list, ft_lstnew(NULL));
		ptr_list = ptr_list->next;
		if (!ptr_list && split_str[i])
		{
			ft_lstclear(&line_list, free);
			free_n_0(NULL, split_str);
			return (NULL);
		}
	}
	free_n_0(NULL, split_str);



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
