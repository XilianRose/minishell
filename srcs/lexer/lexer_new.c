/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_new.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/06 15:08:39 by mstegema      #+#    #+#                 */
/*   Updated: 2023/12/06 15:12:01 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*tokenisation(const char *user_input)
{
	t_list	*tokens;
	char	**ui_array;

	tokens = NULL;
	ui_array = ft_split(user_input, ' ');
	if (!ui_array)
		return (NULL);
	if (make_tlist((const char **) ui_array, &tokens) == 1)
		return (free(ui_array), NULL);
	free(ui_array);
	if (split_rdrtokens(tokens, 0) == EXIT_FAILURE)
		return (free_tokenlst(&tokens), NULL);
	return (tokens);
}