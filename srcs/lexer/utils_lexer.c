/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils_lexer.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/23 11:31:00 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/24 16:19:00 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*quote_begin(t_list *tokens)
{
	t_token	*token;
	size_t	len;

	while (tokens != NULL)
	{
		token = tokens->content;
		len = ft_strlen(token->data) - 1;
		if ((token->data[0] == '\'' || token->data[0] == '\"')
			&& (token->data[len] != '\'' || token->data[len] != '\"'))
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

t_list	*quote_end(t_list *tokens)
{
	t_token	*token;
	size_t	len;

	while (tokens != NULL)
	{
		token = tokens->content;
		len = ft_strlen(token->data) - 1;
		if ((token->data[0] != '\'' || token->data[0] != '\"')
			&& (token->data[len] == '\'' || token->data[len] == '\"'))
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

t_token	*new_token(const char *data, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->data = (char *)data;
	token->type = type;
	return (token);
}
