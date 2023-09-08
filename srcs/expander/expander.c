/* ************************************************************************** */
/*							                                                  */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/30 12:08:20 by mstegema      #+#    #+#                 */
/*   Updated: 2023/09/06 15:39:23 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expanded_part(char *str, t_env *env)
{
	char	**env_array;
	char	*res;
	size_t	i;
	size_t	len;

	env_array = env->new_env;
	res = NULL;
	i = 0;
	while (env_array[i] != NULL)
	{
		len = ft_strlen(env_array[i]);
		if (ft_strnstr(env_array[i], &str[1], len) != NULL)
		{
			len = len - ft_strlen(str);
			res = ft_substr(env_array[i], ft_strlen(str), len);
			return (free(str), res);
		}
		i++;
	}
	return (free(str), res);
}

char	*expand_data(char *str, t_env *env)
{
	char	*new_data;
	char	*temp;
	char	*beginning;
	char	*middle;
	char	*end;

	beginning = find_begin(str);
	if (!beginning)
		return (NULL);
	end = find_end(str);
	if (!end)
		return (free(beginning), NULL);
	middle = find_middle(str);
	if (!middle)
		return (free(beginning), free(end), NULL);
	middle = expanded_part(middle, env);
	if (!middle)
		return (free(beginning), free(end), NULL);
	temp = ft_strjoin(beginning, middle);
	if (!temp)
		return (multi_free(beginning, middle, end, NULL), NULL);
	new_data = ft_strjoin(temp, end);
	if (!new_data)
		return (multi_free(beginning, middle, end, temp), NULL);
	return (new_data);
}

static size_t	replace_token(t_token *token, t_env *env)
{
	char	*new_data;
	char	*temp;

	new_data = expand_data(token->data, env);
	temp = token->data;
	token->data = new_data;
	free(temp);
	return (0);
}

bool	expand_check(char *str, size_t start)
{
	bool	expand;
	size_t	i;
	size_t	count;

	expand = true;
	i = 0;
	count = 0;
	while (str[i] != '\0' || i < start)
	{
		if (str[i] == '\'')
			count++;
		if (str[i] == '$')
			break ;
		i++;
	}
	if (count % 2 != 0)
		expand = false;
	return (expand);
}

size_t	expand(t_list *tokens, t_env *env)
{
	t_token	*token;
	size_t	i;

	while (tokens != NULL)
	{
		i = 0;
		token = tokens->content;
		while (token->data[i] != '\0')
		{
			if (token->data[i] == '$')
			{
				if (expand_check(token->data, i) == true)
					replace_token(token, env);
			}
			i++;
		}
		tokens = tokens->next;
	}
	return (0);
}
