/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 09:42:16 by vrogiste          #+#    #+#             */
/*   Updated: 2022/02/02 10:28:37 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_strlen(const char *str)
{
	if (str && *str)
		return (1 + ft_strlen(str + 1));
	return (0);
}

int	ft_putnbr_count(long long int nb, const char *base)
{
	if (nb < 0)
	{
		write(1, "-", 1);
		return (1 + ft_putnbr_count(-nb, base));
	}
	if (nb >= (long long int)ft_strlen(base))
		return (ft_putnbr_count(nb / ft_strlen(base), base) + write(1, &base[nb % ft_strlen(base)], 1));
	return (write(1, &base[nb % ft_strlen(base)], 1));
}

int	ft_putaddress_count(unsigned long long int nb)
{
	char	*base;

	base = "0123456789abcdef";
	if (nb >= (unsigned long long int)ft_strlen(base))
		return (ft_putnbr_count(nb / ft_strlen(base), base) + write(1, &base[nb % ft_strlen(base)], 1));
	return (write(1, &base[nb % ft_strlen(base)], 1));
}

int	ft_format(char c, va_list args)
{
	if (c == 'c')
	{
		int	x = va_arg(args, int);
		return (write(1, &x, 1));
	}
	if (c == 's')
	{
		char	*str = va_arg(args, char *);
		if (!str)
			return (write(1, "(null)", 6));
		return (write(1, str, ft_strlen(str)));
	}
	if (c == 'd' || c == 'i')
		return (ft_putnbr_count(va_arg(args, int), "0123456789"));
	if (c == 'u')
		return (ft_putnbr_count(va_arg(args, unsigned int), "0123456789"));
	if (c == 'x')
		return (ft_putnbr_count(va_arg(args, unsigned int), "0123456789abcdef"));
	if (c == 'X')
		return (ft_putnbr_count(va_arg(args, unsigned int), "0123456789ABCDEF"));
	if (c == 'p')
		return (write (1, "0x", 2) + ft_putaddress_count(va_arg(args, unsigned long long int)));
	if (c == '%')
		return (write (1, "%", 1));
	return (-1);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		o_len;

	va_start(args, str);
	o_len = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			o_len += ft_format(*(str++), args);
			continue ;
		}
		write(1, str++, 1);
		o_len++;
	}
	va_end(args);
	return (o_len);
}
