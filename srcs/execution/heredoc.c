/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/04 14:59:07 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/24 16:09:21 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_expand(void)
{
	return ;
}

static void	ft_read_input(char *data, int datalen, int *fd, bool expand)
{
	char	*temp;
	int		templen;

	while (1)
	{
		temp = readline("> ");
		if (!temp)
		{
			perror("minishell :");
			break ;
		}
		if (!ft_strncmp(temp, data, datalen))
		{
			free(temp);
			break ;
		}
		if (expand == true)
			ft_expand();
		templen = ft_strlen(temp);
		write(fd[1], temp, templen);
		write(fd[1], "\n", 1);
		free(temp);
	}
}

static void	ft_remove_quotes(char *data)
{
	int	i;
	int	j;

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

void	ft_heredoc(char *data)
{
	int		fd[2];
	int		datalen;
	bool	expand;

	expand = true;
	if (pipe(fd) == -1)
	{
		perror("minishell: ");
		return ;
	}
	datalen = ft_strlen(data);
	if (ft_strchr(data, '"') || ft_strchr(data, '\''))
	{
		ft_remove_quotes(data);
		expand = false;
	}
	ft_read_input(data, datalen, fd, expand);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		perror("minishell :");
	close(fd[0]);
	close(fd[1]);
}
