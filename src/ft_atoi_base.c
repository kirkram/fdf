/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 15:59:06 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/18 11:32:47 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Assignment name  : ft_atoi_base
// Expected files   : ft_atoi_base.c
// Allowed functions: None
// --------------------------------------------------------------------------------

// Write a function that converts the string argument str (base N <= 16)
// to an integer (base 10) and returns it.

// The characters recognized in the input are: 0123456789abcdef
// Those are, of course, to be trimmed according to the requested base. For
// example, base 4 recognizes "0123" and base 16 recognizes "0123456789abcdef".

// Uppercase letters must also be recognized: "12fdb3" is the same as "12FDB3".
// should be 1244595

// Minus signs ('-') are interpreted only if they are the first character of the
// string.

// Your function must be declared as follows:

// int	ft_atoi_base(const char *str, int str_base);

#include "../lib/libft/libft.h"

int		ft_atoi_base(const char *str, int str_base);
int		is_valid_str(char *str, char *set);
size_t	ft_strlen(const char *str);
int		convert_to_dec(char character);
int		make_power(int str_base, int exponent);


int	ft_atoi_base(const char *str, int str_base)
{
	char set[17] = "0123456789abcdef";
	int sign = 1;
	int result;
	int i;
	int len;

	result = 0;
	set[str_base] = '\0';
	if (!str || str_base == 0 || !is_valid_str((char *)str, set))
		return (0);
	i = 0;
	if (str[0] == '-')
		sign = -1;
	len = ft_strlen(str);
	while (str[i])
	{
		result = result + convert_to_dec(str[len - i - 1]) * make_power(str_base, i);
		i ++;
	}
	return (result * sign);
}

int	is_valid_str(char *str, char *set)
{

	int i;
	char *set_upper = "0123456789ABCDEF";

	if (*str == '-')
		str ++;
	while (*str)
	{
		i = 0;
		while (set[i])
		{
			if (*str == set[i] || *str == set_upper[i])
			   break;
			i ++;
		}
		if (set[i] == '\0')
			return (0);
		str ++;
	}
	return (1);
}

int convert_to_dec(char character)
{
	char *set = "0123456789abcdef";
	char *set_upper = "0123456789ABCDEF";
	int i;

	if (character == '-')
		return (0);
	i = 0;
	while (set[i] != character && set_upper[i] != character && set[i])
		i++;
	return (i);
}

int	make_power(int str_base, int exponent)
{
	int result;

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





