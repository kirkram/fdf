/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano@student.hive.fi <klukiano>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/13 11:53:53 by klukiano          #+#    #+#             */
/*   Updated: 2023/11/14 15:52:40 by klukiano@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_putstr(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!str)
	{
		i = write(1, "(null)", 6);
		return (i);
	}
	else
	{
		while (str [i] != '\0')
		{
			j = write(1, &str[i], 1);
			if (j == -1)
				return (-1);
			i++;
		}
	}
	return (i);
}
