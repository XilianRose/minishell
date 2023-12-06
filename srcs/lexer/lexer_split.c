/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_split.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/06 15:12:18 by mstegema      #+#    #+#                 */
/*   Updated: 2023/12/06 15:35:00 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_wordcount(char const *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (ft_strchr(" \'\"", s[i]) == NULL && quote_check(s, i) == NOT_QUOTED)
		{
			count++;
			while (s[i] && ft_strchr(" \'\"", s[i]) == NULL && \
			quote_check(s, i) == NOT_QUOTED)
				i++;
		}
		else
			i++;
	}
	return (count);
}

static int	ft_wordlen(char const *s, char c, int i)
{
	int	len;

	len = 0;
	while (s[i] != c && s[i])
	{
		len++;
		i++;
	}
	return (len);
}

static char	**ft_free(char **ans, int j)
{
	while (j >= 0)
	{
		free(ans[j]);
		j--;
	}
	free(ans);
	return (NULL);
}

char	**lexer_split(char const *s)
{
	int		i;
	int		j;
	int		strings;
	int		len;
	char	**res;

	i = 0;
	j = 0;
	strings = ft_wordcount(s);
	res = (char **)ft_calloc(strings + 1, sizeof(char *));
	if (!res)
		return (NULL);
	while (j < strings)
	{
		while (s[i] == c)
			i++;
		len = ft_wordlen(s, c, i);
		res[j] = ft_substr(s, i, len);
		if (!res[j])
			return (ft_free(res, j));
		i += len;
		j++;
	}
	res[j] = 0;
	return (res);
}