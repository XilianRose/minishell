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

// static char	*ft_join_arr(char **arr, size_t i)
// {
// 	char	*final;
// 	char	*temp;

// 	final = NULL;
// 	if (!arr[i + 1])
// 	{
// 		final = ft_substr(arr[i], 0, ft_strlen(arr[i]));
// 		return (final);
// 	}
// 	while (arr[i + 1])
// 	{
// 		temp = ft_strjoin(arr[i], " ");
// 		if (!temp)
// 			return (NULL);
// 		free(arr[i]);
// 		arr[i] = temp;
// 		i++;
// 	}
// 	i = 0;
// 	while (arr[i])
// 	{
// 		temp = ft_strjoin(arr[i], arr[i + 1]);
// 		if (!temp)
// 			return (NULL);
// 		if (arr[i + 2])
// 		{
// 			free(arr[i + 1]);
// 			arr[i + 1] = temp;
// 		}
// 		else
// 		{
// 			final = temp;
// 			break ;
// 		}
// 		i++;
// 	}
// 	return (final);
// }

// static char	*ft_expand_loop(t_init *process, char *str, size_t i)
// {
// 	char	**arr;
// 	char	*expanded;

// 	arr = ft_split(str, ' ');
// 	if (!arr)
// 		return (ft_throw_error(process, ENOMEM), process->must_exit = true, NULL);
// 	while (arr[i])
// 	{
// 		expanded = expand_data(arr[i], process->env, true, process);
// 		if (!expanded)
// 			return (ft_free_str_array(arr, NULL), ft_throw_error(process, ENOMEM), process->must_exit = true, NULL);
// 		free(arr[i]);
// 		arr[i] = expanded;
// 		i++;
// 	}
// 	expanded = ft_join_arr(arr, 0);
// 	if (!expanded)
// 	{
// 		ft_throw_error(process, ENOMEM);
// 		process->must_exit = true;
// 	}
// 	ft_free_str_array(arr, NULL);
// 	return (expanded);
// }

// static bool	ft_expand_check(t_init *process, int32_t *fd, \
// 	bool expand, char *str)
// {
// 	char	*final;

// 	if (expand == true && ft_strchr(str, '$'))
// 	{
// 		final = ft_expand_loop(process, str, 0);
// 		if (!final)
// 			return (false);
// 		if (write(fd[1], final, ft_strlen(final)) == -1 || \
// 			write(fd[1], "\n", 1) == -1)
// 			perror("BabyBash");
// 		free(final);
// 		return (true);
// 	}
// 	if (write(fd[1], str, ft_strlen(str)) == -1 || \
// 		write(fd[1], "\n", 1) == -1)
// 		perror("BabyBash");
// 	return (true);
// }


static char	*ft_expand_loop(t_init *process, char *str)
{
	char	*temp;
	char	*final;

	final = expand_data(str, process->env, true, process);
	if (!final)
		return (ft_throw_error(process, ENOMEM), \
			process->must_exit = true, NULL);
	while (ft_strchr(final, '$'))
	{
		temp = expand_data(final, process->env, true, process);
		free(final);
		if (!temp)
			return (ft_throw_error(process, ENOMEM), \
				process->must_exit = true, NULL);
		final = malloc(sizeof(char) * (ft_strlen(temp) + 1));
		if (!final)
			return (ft_throw_error(process, ENOMEM), free(temp), \
				process->must_exit = true, NULL);
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
			perror("BabyBash");
		free(final);
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
		process->errorcode = 1;
		if (close(fd[0]) == -1 || close(fd[1]) == -1)
			ft_throw_error(process, errno);
		return (false);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1 || close(fd[0]) == -1 || \
		close(fd[1]) == -1)
		ft_throw_error(process, errno);
	return (true);
}
