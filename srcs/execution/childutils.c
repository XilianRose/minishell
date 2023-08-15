/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   childutils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 16:49:24 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/15 16:55:36 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_prepare_child(t_childproc *child, t_list *lst)
{
	child->errorcode = 0;
	child->oldout = -1;
	child->pipes = NULL;
	child->i = 0;
	child->pipe_count = ft_count_pipes(lst);
	child->nr_of_cmds = child->pipe_count + 1;
	child->ids = malloc(child->nr_of_cmds * sizeof(pid_t));
	if (!child->ids)
		return (perror("minishell: "), false);
	if (child->pipe_count)
	{
		child->pipes = ft_create_pipes(child->pipes, child->pipe_count);
		if (!child->pipes)
		{
			free(child->ids);
			child->ids = NULL;
			return (false);
		}
	}
	return (true);
}
