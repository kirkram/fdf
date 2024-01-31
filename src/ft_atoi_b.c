/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 15:59:06 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/31 12:21:02 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"

unsigned int	ft_atoi_b(const char *str, int str_base);
int				is_valid_str(char *str, char *set);
size_t			ft_strlen(const char *str);
int				convert_to_dec(char character);
long			make_power(long str_base, long exponent);

unsigned int	ft_atoi_b(const char *str, int str_base)
{
	char			*set;
	unsigned int	result;
	int				i;
	int				len;

	set = ft_strdup("0123456789abcdef");
	result = 0;
	set[str_base] = '\0';
	if (!str || str_base == 0 || !is_valid_str((char *)str, set))
		return (0);
	i = 0;
	len = ft_strlen(str);
	while (str[i])
	{
		result += convert_to_dec(str[len - i - 1]) * make_power(str_base, i);
		i ++;
	}
	free (set);
	return (result);
}

int	is_valid_str(char *str, char *set)
{
	int		i;
	char	*set_upper;

	set_upper = "0123456789ABCDEF";
	if (*str == '-')
		str ++;
	while (*str)
	{
		i = 0;
		while (set[i])
		{
			if (*str == set[i] || *str == set_upper[i])
				break ;
			i ++;
		}
		if (set[i] == '\0')
			return (0);
		str ++;
	}
	return (1);
}

int	convert_to_dec(char character)
{
	char	*set;
	char	*set_upper;
	int		i;

	set = "0123456789abcdef";
	set_upper = "0123456789ABCDEF";
	if (character == '-')
		return (0);
	i = 0;
	while (set[i] != character && set_upper[i] != character && set[i])
		i++;
	return (i);
}

long	make_power(long str_base, long exponent)
{
	long	result;

	if (exponent == 0)
		return (1);
	if (exponent == 1)
		return (str_base);
	result = str_base;
	while (exponent > 1)
	{
		result *= str_base;
		exponent --;
	}
	return (result);
}

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (*str)
	{
		str ++;
		i ++;
	}
	return (i);
}
