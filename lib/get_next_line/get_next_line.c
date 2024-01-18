/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 17:53:32 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/17 13:10:26 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd);
void	handle_buffer(char	**cache, char **buffer, int bytes_read);
char	*handle_cache(char **cache, int bytes_read, int fd);
char	*return_line(char ***cache, size_t i);
char	*handle_read(char **cache, char **buffer, int bytes_read);

char	*get_next_line(int fd)
{
	static char	*cache;
	char		*buffer;
	int			bytes_read;

	if (BUFFER_SIZE < 0)
	{
		if (cache)
			free (cache);
		cache = NULL;
		return (NULL);
	}
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer || BUFFER_SIZE < 0)
	{
		if (cache)
			free (cache);
		cache = NULL;
		return (NULL);
	}
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (!handle_read(&cache, &buffer, bytes_read))
		return (NULL);
	handle_buffer(&cache, &buffer, bytes_read);
	return (handle_cache(&cache, bytes_read, fd));
}

char	*handle_read(char **cache, char **buffer, int bytes_read)
{
	if (bytes_read < 0)
	{
		if (*cache)
			free (*cache);
		*cache = NULL;
		free (*buffer);
		return (NULL);
	}
	if (bytes_read == 0)
	{
		if (!*cache)
		{
			free (*buffer);
			return (NULL);
		}
		if (*cache[0] == '\0')
		{
			free (*cache);
			*cache = NULL;
			free (*buffer);
			return (NULL);
		}
	}
	return ("\127\127\127");
}

void	handle_buffer(char	**cache, char **buffer, int bytes_read)
{
	char		*tmp_cache;

	(*buffer)[bytes_read] = '\0';
	if (!*cache)
	{
		*cache = ft_substr(*buffer, 0, -1);
		free (*buffer);
	}
	else
	{
		tmp_cache = ft_substr(*cache, 0, -1);
		free (*cache);
		*cache = ft_strjoin(tmp_cache, *buffer);
		free (tmp_cache);
		free (*buffer);
	}
}

char	*handle_cache(char **cache, int bytes_read, int fd)
{
	size_t		i;
	char		*line;

	i = 0;
	if (*cache)
	{
		while ((*cache)[i] != '\n' && (*cache)[i] != '\0')
			i ++;
		if ((*cache)[i] == '\n')
			i += 1;
		else
			i = 0;
	}
	if (i == 0 && bytes_read == 0)
	{
		line = ft_substr(*cache, 0, -1);
		free (*cache);
		*cache = NULL;
		return (line);
	}
	else if (i == 0)
		return (get_next_line(fd));
	else
		return (return_line(&cache, i));
}

char	*return_line(char ***cache, size_t i)
{
	char	*line;
	char	*tmp_cache;

	line = ft_substr(**cache, 0, i);
	if (!line)
	{
		free (**cache);
		**cache = NULL;
		return (NULL);
	}
	tmp_cache = ft_substr(**cache, i, -1);
	if (!tmp_cache)
	{
		free (**cache);
		**cache = NULL;
		free (line);
		return (NULL);
	}
	free (**cache);
	**cache = tmp_cache;
	return (line);
}
