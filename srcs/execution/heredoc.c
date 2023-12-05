/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/04 14:59:07 by cschabra      #+#    #+#                 */
/*   Updated: 2023/11/24 15:19:43 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_expand_str(t_init *process, int32_t *fd, char *str, size_t i)
{
	char	*temp;
	char	*temp2;
	size_t	j;

	j = 0;
	temp = ft_substr(str, i, ft_strlen(str + i));
	if (!temp)
		return (false);
	temp2 = expand_data(temp, process->env, true, process);
	free(temp);
	if (!temp2)
		return (false);
	while (temp2[j] && temp2[j] != ' ')
	{
		if (write(fd[1], &temp2[j], 1) == -1)
			perror("BabyBash");
		j++;
	}
	free(temp2);
	return (true);
}

static bool	ft_expand_loop(t_init *process, int32_t *fd, char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] && str[i + 1] != ' ')
			{
				if (!ft_expand_str(process, fd, str, i))
					return (false);
				while (str[i] && str[i] != ' ')
					i++;
				continue ;
			}
		}
		if (write(fd[1], &str[i], 1) == -1)
			perror("BabyBash");
		i++;
	}
	if (write(fd[1], "\n", 1) == -1)
		perror("BabyBash");
	return (true);
}

static bool	ft_expand_check(t_init *process, int32_t *fd, bool expand, char *str)
{
	if (expand == true && ft_strchr(str, '$'))
	{
		if (!ft_expand_loop(process, fd, str))
			return (false);
		return (true);
	}
	if (write(fd[1], str, ft_strlen(str)) == -1 || \
		write(fd[1], "\n", 1) == -1)
		perror("BabyBash");
	return (true);
}

static bool	ft_read_input(t_init *process, char *data, int32_t *fd, \
	bool expand)
{
	char	*str;
	size_t	datalen;

	datalen = ft_strlen(data);
	while (1)
	{
		str = readline("> ");
		if (!str || g_signal == SIGINT || !ft_strncmp(str, data, (datalen + 1)))
		{
			if (!str)
				printf("%s\n", \
					"BabyBash: warning: here-doc delimited by end-of-file");
			free(str);
			break ;
		}
		if (!ft_expand_check(process, fd, expand, str))
			return (free(str), false);
		free(str);
		str = NULL;
	}
	return (true);
}

static void	ft_remove_quotes(char *data)
{
	size_t	i;

	i = 0;
	while (data[i])
	{
		if (data[i] == '\'' || data[i] == '"' || data[i] == '\n')
		{
			while (data[i])
			{
				data[i] = data[i + 1];
				i++;
			}
			i = 0;
		}
		else
			i++;
	}
}

bool	ft_heredoc(t_init *process, char *data)
{
	int32_t	fd[2];
	bool	expand;

	expand = true;
	if (pipe(fd) == -1)
		return (ft_throw_error(process, errno), false);
	if (ft_strchr(data, '"') || ft_strchr(data, '\''))
	{
		ft_remove_quotes(data);
		expand = false;
	}
	if (!ft_read_input(process, data, fd, expand))
	{
		ft_throw_error(process, ENOMEM);
		if (close(fd[0]) == -1 || close(fd[1]) == -1)
			ft_throw_error(process, errno);
		return (false);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0]) == -1 || \
		close(fd[1]) == -1)
		ft_throw_error(process, errno);
	return (true);
}
