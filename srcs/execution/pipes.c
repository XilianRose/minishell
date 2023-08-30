/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 13:32:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/30 18:27:40 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		free(pipes[i]);
		pipes[i] = NULL;
		i++;
	}
	if (pipes)
	{
		free(pipes);
		pipes = NULL;
	}
}

int	ft_count_pipes(t_list *lst)
{
	int		pipe_count;

	pipe_count = 0;
	while (lst->next)
	{
		pipe_count++;
		lst = lst->next;
	}
	return (pipe_count);
}

void	ft_create_pipes(t_init *process, int pipe_count)
{
	int	i;

	i = 0;
	process->pipes = malloc(pipe_count * sizeof(int *));
	if (!process->pipes)
	{
		// free all
		ft_throw_error(errno, "BabyBash: ");
	}
	while (i < pipe_count)
	{
		process->pipes[i] = malloc(2 * sizeof(int));
		if (!process->pipes[i] || pipe(process->pipes[i]) == -1)
		{
			if (!process->pipes[i])
				errno = ENOMEM;
			perror("BabyBash: ");
			ft_close_fds(process);
			ft_free_pipes(process->pipes, pipe_count);
			// free all
			exit(1);
		}
		i++;
	}
}
