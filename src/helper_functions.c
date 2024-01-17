/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 13:05:18 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/16 17:09:15 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"

int	free_n_0(int **int_array, char **str_array)
{
	int	i;

	i = 0;
	if (int_array && *int_array)
		free (*int_array);
	if (str_array)
	{
		while (str_array[i])
		{
			free (str_array[i]);
			i ++;
		}
		free (str_array);
	}
	return (0);
}
