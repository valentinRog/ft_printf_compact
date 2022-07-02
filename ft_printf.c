/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 09:42:16 by vrogiste          #+#    #+#             */
/*   Updated: 2022/07/03 00:23:12 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	str_len(const char *str)
{
	if (str && *str)
		return (1 + str_len(str + 1));
	return (0);
}

static int	putnbr_count(int64_t nb, const char *base)
{
	if (nb < 0)
	{
		write(1, "-", 1);
		return (1 + putnbr_count(-nb, base));
	}
	if ((size_t)nb >= str_len(base))
		return (putnbr_count(nb / str_len(base), base)
			+ write(1, &base[nb % str_len(base)], 1));
	return (write(1, &base[nb % str_len(base)], 1));
}

static int	putaddress_count(uintptr_t nb)
{
	const char	*base;

	base = "0123456789abcdef";
	if (nb >= str_len(base))
		return (putnbr_count(nb / str_len(base), base)
			+ write(1, &base[nb % str_len(base)], 1));
	return (write(1, &base[nb % str_len(base)], 1));
}

static int	format(char c, va_list args)
{
	char	*str;
	char	x;

	if (c == 'c')
	{
		x = va_arg(args, int);
		return (write(1, &x, 1));
	}
	if (c == 'd' || c == 'i')
		return (putnbr_count(va_arg(args, int), "0123456789"));
	if (c == 'u')
		return (putnbr_count(va_arg(args, unsigned), "0123456789"));
	if (c == 'x')
		return (putnbr_count(va_arg(args, unsigned), "0123456789abcdef"));
	if (c == 'X')
		return (putnbr_count(va_arg(args, unsigned), "0123456789ABCDEF"));
	if (c == 'p')
		return (write (1, "0x", 2)
			+ putaddress_count(va_arg(args, uintptr_t)));
	str = va_arg(args, char *);
	if (str)
		return (write(1, str, str_len(str)));
	return (write(1, "(null)", 6));
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		o_len;

	va_start(args, str);
	o_len = 0;
	while (*str)
	{
		if (*str == '%' && *(++str) != '%')
		{
			o_len += format(*(str++), args);
			continue ;
		}
		write(1, str++, 1);
		o_len++;
	}
	va_end(args);
	return (o_len);
}
