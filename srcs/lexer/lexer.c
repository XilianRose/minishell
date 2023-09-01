/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 14:10:44 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/31 18:15:20 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	split_rdrtokens(t_list *tokens)
{
	t_token	*token;
	t_list	*next;
	t_token	*new;
	t_list	*new_node;

	while (tokens != NULL)
	{
		token = tokens->content;
		next = tokens->next;
		new = NULL;
		if (token->type == RDR_TOKEN)
			new = is_splitable(token);
		if (new)
		{
			new_node = ft_lstnew(new);
			tokens->next = new_node;
			new_node->next = next;
		}
		tokens = tokens->next;
	}
	return (0);
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

static t_list	*close_quotes(t_list *begin)
{
	t_list	*append_list;
	t_list	*next;
	t_list	*last;
	t_token	*token;
	char	*user_input;

	append_list = NULL;
	next = begin->next;
	last = NULL;
	token = begin->content;
	user_input = "";
	while (1)
	{
		user_input = ft_strjoin(user_input, readline("> "));
		if (token->data[0] == '\'' && ft_strchr(user_input, '\'') != NULL)
			break;
		else if (token->data[0] == '\"' && ft_strchr(user_input, '\"') != NULL)
			break;
	}
	append_list = tokenisation(user_input);
	if (!append_list)
		return (NULL); //throw error;
	begin->next = append_list;
	last = ft_lstlast(append_list);
	last->next = next;
	return (begin);
}

static size_t	merge_tokens(t_list *tokens)
{
	t_list	*begin;
	t_list	*end;
	t_token	*token;
	t_token	*next_token;

	while (tokens != NULL)
	{
		begin = quote_begin(tokens);
		if (begin == NULL)
			return (1);
		end = quote_end(begin);
		if (end == NULL && begin != NULL)
			merge_tokens(close_quotes(begin)); //check with lldb what happens here
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

// malloc fail > throw error
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
	split_rdrtokens(tokens);
	return (tokens);
}
