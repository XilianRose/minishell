/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/24 17:15:46 by mstegema      #+#    #+#                 */
/*   Updated: 2023/10/06 16:50:31 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	replace_data(t_token *token, char delim)
{
	char	*new_data;
	char	*temp;
	size_t	len;
	size_t	i;

	temp = token->data;
	len = ft_strlen(temp);
	new_data = ft_calloc(len - 1, sizeof(char));
	if (!new_data)
		return (false);
	i = 0;
	while (i < len - 1)
	{
		if (temp == ft_strchr(temp, delim) || temp == ft_strrchr(temp, delim))
			temp++;
		else
		{
			new_data[i] = *temp;
			temp++;
			i++;
		}
	}
	temp = token->data;
	token->data = new_data;
	return (free(temp), true);
}

// if (ft_strnstr(token->data, "<<", 3) == NULL)

void	remove_quotes(t_list *tokens)
{
	t_token	*token;
	size_t	i;
	size_t	replaced;

	while (tokens)
	{
		token = tokens->content;
		i = 0;
		replaced = false;
		if (token->type != HERE_DOC)
		{
			while (token->data[i] != '\0')
			{
				if (token->data[i] == '\'')
					replaced = replace_data(token, '\'');
				else if (token->data[i] == '\"')
					replaced = replace_data(token, '\"');
				if (replaced == true)
					break ;
				else
					i++;
			}
		}
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
