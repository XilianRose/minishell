/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/04 14:59:07 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/11 20:17:37 by cheyennesch   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_expand(void)
{
	return ;
}

static bool	ft_read_input(char *data, int32_t datalen, int32_t *fd, bool expand)
{
	char	*temp;
	size_t	templen;

	while (1)
	{
		temp = readline("> ");
		if (!temp)
			return (false);
		if (!ft_strncmp(temp, data, datalen))
		{
			free(temp);
			break ;
		}
		if (expand == true)
			ft_expand();
		templen = ft_strlen(temp);
		if (write(fd[1], temp, templen) == -1 || write(fd[1], "\n", 1) == -1)
		{
			perror("BabyBash");
			return (free(temp), false);
		}
		free(temp);
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
	size_t	datalen;
	bool	expand;

	expand = true;
	if (pipe(fd) == -1)
		return (ft_throw_error(process, errno), false);
	datalen = ft_strlen(data);
	if (ft_strchr(data, '"') || ft_strchr(data, '\''))
	{
		ft_remove_quotes(data);
		expand = false;
	}
	if (!ft_read_input(data, datalen, fd, expand))
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

// static bool	ft_read_input(t_init *process, char *data, int32_t *fd, bool expand)
// {
// 	char	*temp;
// 	char	*final;
// 	size_t	datalen;

// 	datalen = ft_strlen(data);
// 	while (1)
// 	{
// 		temp = readline("> ");
// 		if (!temp)
// 			return (false);
// 		if (!ft_strncmp(temp, data, (datalen + 1)))
// 		{
// 			free(temp);
// 			break ;
// 		}
// 		if (expand == true && ft_strchr(temp, '$'))
// 		{
// 			final = expand_data(temp, process->env);
// 			free(temp);
// 			if (write(fd[1], final, ft_strlen(final)) == -1 || write(fd[1], "\n", 1) == -1)
// 			{
// 				perror("BabyBash");
// 				return (false);
// 			}
// 			free (final);
// 			final = NULL;
// 		}
// 		else
// 		{
// 			if (write(fd[1], temp, ft_strlen(temp)) == -1 || write(fd[1], "\n", 1) == -1)
// 			{
// 				perror("BabyBash");
// 				return (free(temp), false);
// 			}
// 			free(temp);
// 		}
// 	}
// 	return (true);
// }

// bool	ft_heredoc(t_init *process, char *data)
// {
// 	int32_t	fd[2];
// 	bool	expand;

// 	expand = true;
// 	if (pipe(fd) == -1)
// 		return (ft_throw_error(process, errno), false);
// 	if (ft_strchr(data, '"') || ft_strchr(data, '\''))
// 	{
// 		ft_remove_quotes(data);
// 		expand = false;
// 	}
// 	if (!ft_read_input(process, data, fd, expand))
// 	{
// 		process->errorcode = 1;
// 		if (close(fd[0]) == -1 || close(fd[1]) == -1)
// 			ft_throw_error(process, errno);
// 		return (false);
// 	}
// 	if (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0]) == -1 || \
// 		close(fd[1]) == -1)
// 		return (ft_throw_error(process, errno), false);
// 	return (true);
// }
