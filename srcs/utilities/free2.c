/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free2.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/04 14:11:39 by cschabra      #+#    #+#                 */
/*   Updated: 2023/11/02 16:44:48 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_token(t_token *token)
{
	t_token			*temp;
	t_token_type	*type;

	temp = token;
	// if (temp->data)
	// {
	// 	free(temp->data);
	// 	temp->data = NULL;
	// }
	if (temp->type)
	{
		type = &temp->type;
		free(type);
	}
	token = NULL;
}

void	free_tokenlst(t_list *tokens)
{
	t_list	*temp;
	t_list	*next;

	if (!tokens)
		return ;
	temp = tokens;
	while (temp)
	{
		next = temp->next;
		free_token(temp->content);
		free(temp);
		temp = next;
	}
	tokens = NULL;
}
