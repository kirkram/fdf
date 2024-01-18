/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano@student.hive.fi <klukiano>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 14:17:42 by klukiano@st       #+#    #+#             */
/*   Updated: 2023/11/15 18:04:19 by klukiano@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>
#include <unistd.h>

int		ft_printf(const char *str, ...);
int		traverse_str(const char *str, va_list args, int len);
int		ft_print_arg(char c, va_list args);
int		ft_putnbr(long n, char *base, long len);
int		ft_putnbr_ptr(unsigned long n, char *base, unsigned long len);

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		len;

	va_start(args, str);
	len = 0;
	len = traverse_str(str, args, len);
	va_end(args);
	return (len);
}

int	traverse_str(const char *str, va_list args, int len)
{
	int	error;

	error = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str ++;
			len += ft_print_arg(*(str), args);
			if (error > len)
				return (-1);
			if (!(*(str)) || !(*(str + 1)))
				break ;
			str ++;
		}
		if (*str != '%')
		{
			len += write (1, &(*str), 1);
			if (error > len)
				return (-1);
			str ++;
		}
		error = len;
	}
	return (len);
}

int	ft_print_arg(char c, va_list args)
{
	int	size;

	size = 0;
	if (c == 's')
		size = ft_putstr(va_arg(args, char *));
	else if (c == 'c')
		size = ft_putchar(va_arg(args, int));
	else if (c == 'p')
	{
		size = ft_putstr("0x");
		if (size == -1)
			return (-1);
		size += ft_putnbr_ptr(va_arg(args, uintptr_t), "0123456789abcdef", 16);
	}
	else if (c == 'x')
		size = ft_putnbr(va_arg(args, unsigned int), "0123456789abcdef", 16);
	else if (c == 'X')
		size = ft_putnbr(va_arg(args, unsigned int), "0123456789ABCDEF", 16);
	else if (c == 'u')
		size = ft_putnbr(va_arg(args, unsigned int), "0123456789", 10);
	else if (c == 'd' || c == 'i')
		size = ft_putnbr(va_arg(args, int), "0123456789", 10);
	else if (c == '%')
		size += write(1, "%", 1);
	return (size);
}

int	ft_putnbr(long n, char *base, long base_len)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		n = -n;
		i = write(1, "-", 1);
		if (i == -1)
			return (-1);
	}
	if (n >= base_len)
	{
		i += ft_putnbr(n / base_len, base, base_len);
		if (i < 0)
			return (-1);
	}
	i += ft_putchar(base[n % base_len]);
	return (i);
}

int	ft_putnbr_ptr(unsigned long n, char *base, unsigned long base_len)
{
	int	i;

	i = 0;
	if (n >= base_len)
		i += ft_putnbr(n / base_len, base, base_len);
	i += ft_putchar(base[n % base_len]);
	if (i < 0)
		return (-1);
	return (i);
}
