/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/24 17:15:46 by mstegema      #+#    #+#                 */
/*   Updated: 2023/11/09 13:58:29 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	replace_data(t_token *token)
{
	char	*new_data;
	char	*temp;
	size_t	i;
	bool	in_single;
	bool	in_double;

	temp = token->data;
	new_data = ft_calloc(ft_strlen(temp) - 1, sizeof(char));
	if (!new_data)
		return (EXIT_FAILURE);
	i = 0;
	while (*temp != '\0')
	{
		if (*temp == '\'' && in_double == false)
			in_single = !in_single;
		else if (*temp == '\"' && in_single == false)
			in_double = !in_double;
		else
			new_data[i++] = *temp;
		temp++;
	}
	temp = token->data;
	token->data = new_data;
	return (free(temp), EXIT_SUCCESS);
}

size_t	remove_quotes(t_list *tokens)
{
	t_token	*token;
	size_t	i;

	while (tokens)
	{
		token = tokens->content;
		i = 0;
		if (token->data != NULL)
		{
			if (ft_strnstr(token->data, "<<", 3) != NULL)
				tokens = tokens->next;
			else if (ft_strchr(token->data, '\'') != NULL \
			|| ft_strchr(token->data, '\"') != NULL)
			{
				if (replace_data(token) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
		}
		if (tokens)
			tokens = tokens->next;
	}
	return (EXIT_SUCCESS);
}

size_t	count_cmdtokens(t_list **tokens)
{
	t_list	*current;
	t_token	*token;
	size_t	count;

	current = *tokens;
	token = current->content;
	count = 0;
	while (token->type != PIPE_TOKEN && current != NULL)
	{
		token = current->content;
		if (token->type == CMD_TOKEN)
			count++;
		else if (token->type == RDR_TOKEN)
			current = current->next;
		if (current != NULL)
			current = current->next;
	}
	return (count);
}

bool	is_builtin(t_list **tokens)
{
	t_list	*current;
	t_token	*token;
	char	*str;

	current = *tokens;
	token = current->content;
	str = token->data;
	if ((ft_strncmp(str, "echo", 5) == 0) || (ft_strncmp(str, "cd", 3) == 0) \
	|| (ft_strncmp(str, "pwd", 4) == 0) || (ft_strncmp(str, "export", 7) == 0) \
	|| (ft_strncmp(str, "unset", 6) == 0) || (ft_strncmp(str, "env", 4) == 0) \
	|| (ft_strncmp(str, "exit", 5) == 0))
		return (true);
	return (false);
}
