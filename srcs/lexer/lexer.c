/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 14:10:44 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/23 14:57:37 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array != NULL && array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	if (array != NULL)
	{
		free(array);
		array = NULL;
	}
}

void	print_array(char **array)
{
	while (array[0] != '\0')
	{
		printf("array str: [%s]\n", *array);
		array++;
	}
}

void	print_tlist(t_list *list)
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

static t_list	*quote_end(t_list *tokens, size_t len)
{
	t_token	*token;

	while (tokens != NULL)
	{
		token = tokens->content;
		if ((token->data[0] != '\'' || token->data[0] != '\"')
			&& (token->data[len] == '\'' || token->data[len] == '\"'))
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

	// find starting quotes & add pointer to node
	// find ending quotes & add pointer to node
	// while != endnode > new_data = strjoin(beginnode.data, nextnode.data))
	// beginnode.data = new_data
	// beginnode.next = endnode.next
	// while != endnode.next > lst_delone
static size_t	merge_tokens(t_list *tokens)
{
	t_list	*begin;
	t_list	*end;
	t_token	*token;
	size_t	len;

	while (tokens != NULL)
	{
		token = tokens->content;
		len = ft_strlen(token->data) - 1;
		if ((token->data[0] == '\'' || token->data[0] == '\"')
			&& (token->data[len] != '\'' || token->data[len] != '\"'))
		{
			begin = tokens;
			end = quote_end(tokens, len);
			if (!end)
				exit (1);
			token = begin->content;
			while (tokens != end)
			{
				token->data = ft_strjoin(token->data, )
			}

		}
		tokens = tokens->next;
	}

}

t_token	*new_token(const char *data, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token *));
	if (token == NULL)
		return (NULL);
	token->data = (char *)data;
	token->type = type;
	return (token);
}

static t_token	*init_token(const char *str)
{
	if (ft_strncmp(str, "|", 2) == 0)
		return (new_token(str, PIPE_TOKEN));
	else if ((ft_strncmp(str, ">", 2) == 0) || (ft_strncmp(str, "<", 2) == 0)
		|| (ft_strncmp(str, ">>", 3) == 0) || (ft_strncmp(str, "<<", 3) == 0))
		return (new_token(str, REDIRECTION_TOKEN));
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

t_list	*tokenisation(const char *user_input)
{
	t_list	*tokens;
	char	**ui_array;

	tokens = NULL;
	ui_array = ft_split(user_input, ' ');
	if (!ui_array)
		exit(1); //exit "failed to parse"?
	print_array(ui_array);
	make_tlist((const char **) ui_array, &tokens);
	//error handling -> exit "failed to parse"?
	print_tlist(tokens);
	return (free_array(ui_array), tokens);
}

int	main(void)
{
	char	*user_input;

	user_input = "cat << \";\" | grep \"aap noot mies\" | wc -l > outfile | >l";
	tokenisation(user_input);
	return (0);
}
