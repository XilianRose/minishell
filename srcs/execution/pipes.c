/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 13:32:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/04 16:23:57 by cschabra      ########   odam.nl         */
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

bool	ft_create_pipes(t_init *process, size_t pipe_count)
{
	size_t	i;

	i = 0;
	process->pipes = malloc(pipe_count * sizeof(int *));
	if (!process->pipes)
		return (perror("BabyBash"), false);
	while (i < pipe_count)
	{
		process->pipes[i] = malloc(2 * sizeof(int));
		if (!process->pipes[i] || pipe(process->pipes[i]) == -1)
		{
			perror("BabyBash");
			ft_free_pipes(process->pipes, i);
			return (false);
		}
		i++;
	}
	return (true);
}
