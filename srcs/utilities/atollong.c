/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   atollong.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 13:50:56 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/04 16:52:05 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_whitespace_ll(char c)
{
	if (c == ' ' || c == '\t'
		|| c == '\r' || c == '\n'
		|| c == '\v' || c == '\f')
		return (1);
	return (0);
}

static int	skip_white_space_ll(const char *str, int *multiplyer)
{
	int		i;
	char	c;
	int		is_prefixed;

	i = 0;
	is_prefixed = 0;
	c = str[i];
	while ((is_whitespace_ll(c) == 1 || c == '-' || c == '+')
		&& is_prefixed == 0)
	{
		if (c == '-' || c == '+')
		{
			if (c == '-')
				*multiplyer = -1;
			is_prefixed = 1;
		}
		i++;
		c = str[i];
	}
	if (ft_isdigit(c) == 0)
		return (-1);
	return (i);
}

long long int	ft_atollong(t_list *lst, t_init *process, const char *str)
{
	unsigned long long int	result;
	long long int			return_result;
	int						i;
	char					c;
	int						multiplyer;

	result = 0;
	multiplyer = 1;
	i = skip_white_space_ll(str, &multiplyer);
	if (i == -1)
		return (0);
	c = str[i];
	while (ft_isdigit(c) == 1)
	{
		result += (c - 48);
		if (ft_isdigit(str[i + 1]) == 1)
			result *= 10;
		i++;
		c = str[i];
	}
	if (((result - 1) > LLONG_MAX && multiplyer == -1) \
		|| (result > LLONG_MAX && multiplyer == 1))
		ft_error_exit(lst, process, (char *)str);
	return_result = result * multiplyer;
	return (return_result);
}
