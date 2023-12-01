/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 14:10:44 by mstegema      #+#    #+#                 */
/*   Updated: 2023/12/01 15:17:26 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	split_rdrtokens(t_list *tokens, size_t i)
{
	t_list	*next;
	t_token	*new;
	t_list	*new_node;

	while (tokens != NULL)
	{
		next = tokens->next;
		if (((t_token *)(tokens->content))->type == RDR_TOKEN)
		{
			i = is_splitable((t_token *)(tokens->content));
			if (i < ft_strlen(((t_token *)(tokens->content))->data))
			{
				new = split_rdrtoken((t_token *)(tokens->content), i);
				if (!new)
					return (EXIT_FAILURE);
				new_node = ft_lstnew(new);
				if (!new_node)
					return (free(new), EXIT_FAILURE);
				tokens->next = new_node;
				new_node->next = next;
			}
		}
		tokens = tokens->next;
	}
	return (EXIT_SUCCESS);
}

static void	print_tlist(t_list *list)
{
	t_token	*current;

	current = list->content;
	while (list != NULL && list->next != NULL)
	{
		printf("data: [%s]	type: [%d]\n", current->data, current->type);
		list = list->next;
		current = list->content;
	}
	if (list != NULL)
		printf("data: [%s]	type: [%d]\n", current->data, current->type);
}

// static void	delete_merged_nodes(t_list **tokens, t_list *begin, t_list *end)
// {
// 	t_list	*current;

// 	current = *tokens;
// 	while (current != begin)
// 		current = current->next;
// 	while (current != end && begin->next != NULL)
// 	{
// 		begin->next = current->next;
// 		ft_lstdelone(current, &free);
// 		current = begin->next;
// 	}
// }

static size_t	merge_tokens(t_list *tokens)
{
	t_list	*begin;
	t_list	*end;

	while (tokens != NULL)
	{
		begin = quote_begin(tokens);
		if (begin == NULL)
			return (EXIT_SUCCESS);
		end = quote_end(begin);
		if (end == NULL)
			return (EXIT_SUCCESS);
		tokens = begin;
		if (join_datastr(tokens, end) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		print_tlist(tokens);
		begin->next = end->next;
		tokens = tokens->next;
	}
	return (EXIT_SUCCESS);
}

// static size_t	merge_tokens(t_list **tokens)
// {
// 	t_list	*begin;
// 	t_list	*end;
// 	t_list	*current;

// 	current = *tokens;

// 	while (current != NULL)
// 	{
// 		begin = quote_begin(current);
// 		if (begin == NULL)
// 			return (EXIT_SUCCESS);
// 		end = quote_end(begin);
// 		if (end == NULL)
// 			return (EXIT_SUCCESS);
// 		current = begin;
// 		if (join_datastr(current, end) == EXIT_FAILURE)
// 			return (EXIT_FAILURE);
// 		delete_merged_nodes(tokens, current, end);
// 		current = end->next;
// 	}
// 	return (EXIT_SUCCESS);
// }

static void	init_token(t_list *tokens)
{
	t_token	*token;

	token = NULL;
	while (tokens != NULL)
	{
		token = tokens->content;
		if (ft_strncmp(token->data, "|", 2) == 0)
			token->type = PIPE_TOKEN;
		else if (ft_strchr(token->data, '\"') == NULL && \
				ft_strchr(token->data, '\'') == NULL && \
				((ft_strchr(token->data, '>') != NULL) || \
				(ft_strchr(token->data, '<') != NULL)) && \
				(ft_strncmp(token->data, "\\<", 3) != 0))
			token->type = RDR_TOKEN;
		else
			token->type = CMD_TOKEN;
		tokens = tokens->next;
	}
}

static size_t	make_tlist(const char **ui_array, t_list **tokens)
{
	t_list	*node;
	t_token	*token;
	char	*str;

	while (ui_array[0] != NULL)
	{
		str = (char *)*ui_array;
		token = new_token(str);
		if (token == NULL)
			return (free_tokenlst(*tokens), 1);
		node = ft_lstnew(token);
		if (node == NULL)
			return (free_tokenlst(*tokens), 1);
		ft_lstadd_back(tokens, node);
		ui_array++;
	}
	return (0);
}

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
	print_tlist(tokens);
	free(ui_array);
	if (merge_tokens(tokens) == EXIT_FAILURE)
		return (free_tokenlst(tokens), NULL);
	print_tlist(tokens);
	init_token(tokens);
	if (split_rdrtokens(tokens, 0) == EXIT_FAILURE)
		return (free_tokenlst(tokens), NULL);
	return (tokens);
}
