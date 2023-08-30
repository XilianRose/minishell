/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 14:10:44 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/30 15:20:38 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	merge_tokens(t_list *tokens)
{
	t_list	*begin;
	t_list	*end;
	t_token	*token;
	t_token	*next_token;

	while (tokens != NULL)
	{
		begin = quote_begin(tokens);
		end = quote_end(begin);
		if (begin == NULL || end == NULL)
			return (1); //no quotes || no ending quotes
		token = begin->content;
		tokens = begin;
		while (tokens != end)
		{
			next_token = tokens->next->content;
			token->data = ft_strjoin(token->data, " ");
			token->data = ft_strjoin(token->data, next_token->data);
			tokens = tokens->next;
		}
		begin->next = end->next;
		tokens = tokens->next;
	}
	return (0);
}

static t_token	*init_token(const char *str)
{
	if (ft_strncmp(str, "|", 2) == 0)
		return (new_token(str, PIPE_TOKEN));
	else if ((ft_strncmp(str, ">", 2) == 0) || (ft_strncmp(str, "<", 2) == 0)
		|| (ft_strncmp(str, ">>", 3) == 0) || (ft_strncmp(str, "<<", 3) == 0))
		return (new_token(str, RDR_TOKEN));
	else
		return (new_token(str, CMD_OR_FILE_TOKEN));
}

static size_t	make_tlist(const char **ui_array, t_list **tokens)
{
	t_list	*node;
	t_token	*token;
	char	*str;

	while (ui_array[0] != '\0')
	{
		str = (char *)*ui_array;
		token = init_token(str);
		if (token == NULL)
			return (ft_lstclear(tokens, &free), 1);
		node = ft_lstnew(token);
		if (node == NULL)
			return (ft_lstclear(tokens, &free), 1);
		ft_lstadd_back(tokens, node);
		ui_array++;
	}
	return (0);
}

// <infile (all rdr) should still work without space delimiter
t_list	*tokenisation(const char *user_input)
{
	t_list	*tokens;
	char	**ui_array;

	tokens = NULL;
	ui_array = ft_split(user_input, ' ');
	if (!ui_array)
		exit(1); //exit "failed to parse"?
	make_tlist((const char **) ui_array, &tokens);
	merge_tokens(tokens);
	return (tokens);
}
