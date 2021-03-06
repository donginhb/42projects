/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 02:08:51 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/05/02 06:04:45 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "ft_base_printf.h"

static int		pf_atoi(t_dt *data)
{
	int			res;

	res = 0;
	while (*data->tail && ft_isdigit(*data->tail))
	{
		res = res * 10;
		res = res + *data->tail - '0';
		data->tail++;
	}
	data->tail--;
	return (res);
}

static int		get_more_options(t_dt *data)
{
	if (*data->tail == 'h')
	{
		if (data->flag.len_modifier & ARG_H)
			data->flag.len_modifier |= ARG_HH;
		else
			data->flag.len_modifier |= ARG_H;
	}
	else if (*data->tail == 'l')
	{
		if (data->flag.len_modifier & ARG_L)
			data->flag.len_modifier |= ARG_LL;
		else
			data->flag.len_modifier |= ARG_L;
	}
	else if (*data->tail == 'j')
		data->flag.len_modifier |= ARG_J;
	else if (*data->tail == 'z')
		data->flag.len_modifier |= ARG_Z;
	else
		return (1);
	return (0);
}

static void		get_options(t_dt *data)
{
	while (*(++data->tail))
	{
		if (*data->tail == '#')
			data->flag.hash = 1;
		else if (*data->tail == '-')
			data->flag.minus = 1;
		else if (*data->tail == '+')
			data->flag.plus = 1;
		else if (*data->tail == ' ')
			data->flag.space = 1;
		else if (*data->tail == '0')
			data->flag.zero = 1;
		else if (ft_isdigit(*data->tail))
			data->flag.min_width = pf_atoi(data);
		else if (*data->tail == '.')
		{
			data->flag.point = data->tail++;
			data->flag.precision = pf_atoi(data);
		}
		else if (get_more_options(data))
			break ;
	}
}

static void		print_flag(t_dt *data)
{
	if (data->flag.minus && *data->tail)
		write_char(data, *data->tail);
	while (data->flag.min_width-- > 1)
		write_char(data, (data->flag.zero) ? '0' : ' ');
	if (!data->flag.minus && *data->tail)
		write_char(data, *data->tail);
	if (!*data->tail)
		data->tail--;
}

int				ft_printf(const char *restrict format, ...)
{
	static void	(*func[])(t_dt *) = { PRINTF_FUNC1, PRINTF_FUNC2 };
	t_dt		data;
	char		*ptr;

	ft_memset(&data, 0, sizeof(data));
	data.tail = (char *)format;
	va_start(data.ap, format);
	while (*data.tail)
	{
		if (*data.tail == '%')
		{
			ptr = NULL;
			get_options(&data);
			if (*data.tail)
				ptr = ft_strchr(PRINTF_ARGS, *data.tail);
			(ptr) ? func[ptr - PRINTF_ARGS](&data) : print_flag(&data);
			ft_memset(&data.flag, 0, sizeof(data.flag));
		}
		else
			write_char(&data, *data.tail);
		data.tail++;
	}
	data.ret += write(STDOUT_FILENO, data.buff, data.pos);
	va_end(data.ap);
	return (data.ret - data.less);
}
