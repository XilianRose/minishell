/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/04 14:59:07 by cschabra      #+#    #+#                 */
/*   Updated: 2023/11/09 15:52:32 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_expand_loop(t_init *process, char *str)
{
	char	*temp;
	char	*final;

	final = expand_data(str, process->env); // test continue here for leaks/right error/msg
	if (!final)
		return (NULL);
	while (ft_strchr(final, '$'))
	{
		temp = expand_data(final, process->env);
		free(final);
		if (!temp)
			return (NULL);
		final = malloc(sizeof(char) * (ft_strlen(temp) + 1));
		if (!final)
			return (perror("BabyBash"), free(temp), NULL);
		ft_memmove(final, temp, ft_strlen(temp) + 1);
		free(temp);
		temp = NULL;
	}
	return (final);
}

static bool	ft_expand_check(t_init *process, int32_t *fd, \
	bool expand, char *str)
{
	char	*final;

	if (expand == true && ft_strchr(str, '$'))
	{
		final = ft_expand_loop(process, str);
		if (!final)
			return (false);
		if (write(fd[1], final, ft_strlen(final)) == -1 || \
			write(fd[1], "\n", 1) == -1)
		{
			perror("BabyBash");
			return (free(final), false);
		}
		free(final);
	}
	else
	{
		if (write(fd[1], str, ft_strlen(str)) == -1 || \
			write(fd[1], "\n", 1) == -1)
		{
			perror("BabyBash");
			return (false);
		}
	}
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
		if (!str)
		{
			printf("%s\n", \
				"BabyBash: warning: here-doc delimited by end-of-file");
			return (free(str), true);
		}
		if (!ft_strncmp(str, data, (datalen + 1)))
		{
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
	int32_t	i;
	int32_t	j;

	i = 0;
	while (data[i])
	{
		if (data[i] == '\'' || data[i] == '"')
		{
			j = i;
			while (data[j])
			{
				data[j] = data[j + 1];
				j++;
			}
		}
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
		process->errorcode = 1;
		if (close(fd[0]) == -1 || close(fd[1]) == -1)
			ft_throw_error(process, errno);
		return (false);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0]) == -1 || \
		close(fd[1]) == -1)
		return (ft_throw_error(process, errno), false);
	return (true);
}
