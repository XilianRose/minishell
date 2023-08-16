/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/16 13:31:39 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/16 13:48:15 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_scmd_list	*make_scmdlist(void)
{
}

size_t	calc_smds(char **ui_array)
{
	size_t	res;
	size_t	i;

	res = 1;
	i = 0;
	while (ui_array)
	{
		if (ui_array[i][0] == '|')
			res++;
		i++;
	}
	return (res);
}

size_t	calc_smds(char **ui_array)
{
	size_t	res;
	size_t	i;

	res = 1;
	i = 0;
	while (ui_array)
	{
		if (ui_array[i][0] == '|')
			res++;
		i++;
	}
	return (res);
}
