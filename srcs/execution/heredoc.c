/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/04 14:59:07 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/22 13:38:49 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_heredoc(char *data)
{
	int		fd[2];
	int		templen;
	int		datalen;
	char	*temp;
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
		// remove quotes from delimiter to use as delimiter
		expand = false;
	}
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
		// if (expand == true)
		// {
		// 	// call expand function that returns the string if found in env else stays the same
		// }
		templen = ft_strlen(temp);
		write(fd[1], temp, templen);
        write(fd[1], "\n", 1);
		free(temp);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1)
		perror("minishell :");
	close(fd[0]);
	close(fd[1]);
}
