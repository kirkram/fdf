/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:14:40 by klukiano          #+#    #+#             */
/*   Updated: 2024/11/06 20:15:18 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# include <unistd.h>
# include <stdlib.h>
# include "../libft/libft.h"

int		get_next_line(char **str, int fd);
void	handle_buffer(char	**cache, char **buffer, int bytes_read);
int		handle_cache(char **str, char **cache, int bytes_read, int fd);
int		return_line(char **str, char ***cache, size_t i);
int		handle_read(char **cache, char **buffer, int bytes_read);

#endif
