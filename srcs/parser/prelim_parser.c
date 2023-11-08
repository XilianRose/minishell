/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prelim_parser.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/04 15:55:15 by mstegema      #+#    #+#                 */
/*   Updated: 2023/11/08 14:09:10 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	userinput_check(char c, char *user_input)
{
	bool	res;

	res = false;
	if (c == '\'' && ft_strchr(user_input, '\'') != NULL)
		res = true;
	else if (c == '\"' && ft_strchr(user_input, '\"') != NULL)
		res = true;
	else if (c == '|' && ft_strlen(user_input) > 0)
		res = true;
	return (res);
}

static char	*new_userinput(char *input, char c)
{
	char	*user_input;
	char	*temp;

	user_input = "";
	temp = "";
	while (1)
	{
		if (c == '|')
			temp = ft_strjoin(user_input, " ");
		else
			temp = ft_strjoin(user_input, "\n");
		if (!temp)
			return (NULL);
		user_input = ft_strjoin(temp, readline("> "));
		free(temp);
		if (!user_input)
			return (NULL);
		if (userinput_check(c, user_input) == true)
			break ;
	}
	temp = input;
	input = ft_strjoin(temp, user_input);
	if (!input)
		return (free(temp), free(user_input), NULL);
	return (free(temp), free(user_input), input);
}

// static char	*new_userinput(char *input, char c)
// {
// 	char	*user_input;
// 	char	*temp;

// 	user_input = "";
// 	temp = "";
// 	while (1)
// 	{
// 		if (c == '|')
// 			temp = ft_strjoin(user_input, " ");
// 		else
// 			temp = ft_strjoin(user_input, "\n");
// 		if (!temp)
// 			return (NULL);
// 		user_input = ft_strjoin(temp, readline("> "));
// 		free(temp);
// 		if (!user_input)
// 			return (NULL);
// 		if (c == '\'' && ft_strchr(user_input, '\'') != NULL)
// 			break ;
// 		else if (c == '\"' && ft_strchr(user_input, '\"') != NULL)
// 			break ;
// 		else if (c == '|' && ft_strlen(user_input) > 0)
// 			break ;
// 	}
// 	temp = input;
// 	input = ft_strjoin(temp, user_input);
// 	if (!input)
// 		return (free(temp), free(user_input), NULL);
// 	return (free(temp), free(user_input), input);
// }

static size_t	needs_closing(char *input, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (input[i] != '\0')
	{
		if (input[i] == c)
			count++;
		if (count == 2)
			return (i);
		i++;
	}
	if (count == 1)
		return (0);
	return (i);
}

static char	*close_quotes(char *input)
{
	size_t	i;
	size_t	check;

	i = 0;
	check = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '\'')
		{
			check = needs_closing(&input[i], '\'');
			if (check == 0)
				return (new_userinput(input, '\''));
			i = i + check;
		}
		else if (input[i] == '\"')
		{
			check = needs_closing(&input[i], '\"');
			if (check == 0)
				return (new_userinput(input, '\"'));
			i = i + check;
		}
		i++;
	}
	return (input);
}

char	*complete_input(t_init *process, char *input)
{
	size_t	len;

	input = close_quotes(input);
	if (!input)
		return (ft_throw_error(process, ENOMEM), NULL);
	len = ft_strlen(input);
	if (!len)
		return (input);
	if (input[len - 1] == '|' || input[len - 2] == '|')
		input = new_userinput(input, '|');
	if (!input)
		return (ft_throw_error(process, ENOMEM), NULL);
	return (input);
}
