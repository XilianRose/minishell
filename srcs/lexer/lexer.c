/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 14:10:44 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/15 15:31:02 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	to par
t_scmd_list	*make_scmdlist(void)
{
}

//	to parser
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

t_list	*make_tlist(char **ui_array, t_list *tokens)
{
	t_list	*token;
	size_t	len;

	len = calc_smds(ui_array);
	ft_lstadd_back(&tokens, token);
}

t_list	*tokenisation(char *user_input)
{
	t_list	*tokens;
	char	**ui_array;

	ui_array = ft_split(user_input, ' ');
	if (!ui_array)
		exit(1);
	return (my_freearray(ui_array), tokens);
}

// int	main(void)
// {

// }
