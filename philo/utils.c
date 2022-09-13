/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 19:01:38 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/12 19:05:29 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int c)
{
	return ((unsigned)c - '0' <= 9);
}

static int	char_is_whitespace(char c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long				out;
	unsigned int		neg;

	out = 0;
	neg = 0;
	while (char_is_whitespace(*str))
		str++;
	if ((*str == '-' || *str == '+') && *str++ == '-')
		neg = 1;
	while (ft_isdigit(*str))
	{
		if (out < 922337203685477580
			|| (out == 922337203685477580 && *str <= '7'))
			out = (out * 10) + (*str++ - '0');
		else
			return (!neg * -1);
	}
	if (neg)
		out = out * -1;
	return ((int)out);
}
