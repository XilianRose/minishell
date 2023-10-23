/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/24 17:15:46 by mstegema      #+#    #+#                 */
/*   Updated: 2023/10/23 11:09:43 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	replace_data(t_token *token)
{
	char	*new_data;
	char	*temp;
	size_t	i;
	bool	in_single;
	bool	in_double;

	temp = token->data;
	new_data = ft_calloc(ft_strlen(temp) - 1, sizeof(char));
	if (!new_data)
		return (false);
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
	return (free(temp), true);
}

void	remove_quotes(t_list *tokens)
{
	t_token	*token;
	size_t	i;

	while (tokens)
	{
		token = tokens->content;
		i = 0;
		if (ft_strnstr(token->data, "<<", 3) != NULL)
			tokens = tokens->next;
		else if (strchr(token->data, '\'') != NULL \
		|| strchr(token->data, '\"') != NULL)
			replace_data(token);
		if (tokens)
			tokens = tokens->next;
	}
}

size_t	count_cmdtokens(t_list **tokens)
{
	t_list	*current;
	t_token	*token;
	size_t	count;

	current = *tokens;
	token = current->content;
	count = 0;
	while (token->type == CMD_OR_FILE_TOKEN && current != NULL)
	{
		token = current->content;
		if (token->type == CMD_OR_FILE_TOKEN)
			count++;
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
