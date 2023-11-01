/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/23 11:31:00 by mstegema      #+#    #+#                 */
/*   Updated: 2023/11/01 17:47:22 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*is_splitable(t_token *token, char *data, size_t len, size_t i)
{
	char	*new_data;
	t_token	*new;

	new_data = NULL;
	new = NULL;
	while (i < len)
	{
		if (((ft_strchr("<", data[i]) != NULL && ft_strchr("<", data[i + 1]) == \
		NULL) || (ft_strchr("<>", data[i]) == NULL && \
		ft_strchr("<>", data[i + 1]) != NULL)) && data[i + 1] != '\0')
			break ;
		i++;
	}
	if (i < len)
		new_data = ft_substr(data, i + 1, len - i);
	if (new_data)
	{
		new = new_token(new_data, RDR_TOKEN);
		token->data = ft_substr(data, 0, i + 1);
		free (data);
	}
	if (!(ft_strchr(token->data, '>')) && !(ft_strchr(token->data, '<')))
		token->type = CMD_TOKEN;
	return (new);
}

size_t	join_datastr(t_list *tokens, t_list *end)
{
	t_token	*token;
	t_token	*next_token;
	char	*temp;

	token = tokens->content;
	while (tokens != end)
	{
		next_token = tokens->next->content;
		temp = ft_strjoin(token->data, " ");
		free(token->data);
		token->data = ft_strjoin(temp, next_token->data);
		free(temp);
		free(next_token->data);
		tokens = tokens->next;
	}
	return (0);
}

t_list	*quote_end(t_list *tokens)
{
	t_token	*token;
	size_t	i;
	char	delim;

	token = tokens->content;
	delim = 0;
	while (tokens != NULL)
	{
		i = 0;
		token = tokens->content;
		while (token->data[i] != '\0')
		{
			if ((token->data[i] == '\'' || token->data[i] == '\"') \
			&& delim == 0)
				delim = token->data[i];
			else if (token->data[i] == delim)
				return (tokens);
			i++;
		}
		tokens = tokens->next;
	}
	return (NULL);
}

t_list	*quote_begin(t_list *tokens)
{
	t_token	*token;
	size_t	i;

	while (tokens != NULL)
	{
		i = 0;
		token = tokens->content;
		while (token->data[i] != '\0')
		{
			if (token->data[i] == '\'' || token->data[i] == '\"')
				return (tokens);
			i++;
		}
		tokens = tokens->next;
	}
	return (NULL);
}

t_token	*init_token(const char *str)
{
	if (ft_strncmp(str, "|", 2) == 0)
		return (new_token(str, PIPE_TOKEN));
	else if ((ft_strchr(str, '>') != NULL) || (ft_strchr(str, '<') != NULL))
		return (new_token(str, RDR_TOKEN));
	else
		return (new_token(str, CMD_TOKEN));
}
