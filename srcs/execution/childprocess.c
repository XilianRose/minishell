/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   childprocess.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 13:32:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/10 16:58:37 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_store_output(t_childproc *child)
{
	child->oldout = dup(STDOUT_FILENO);
	if (child->oldout == -1)
	{
		ft_putstr_fd("minishell: old fd dup failed.", STDERR_FILENO);
		return (false);
	}
	return (true);
}

static void	ft_wait_for_last_child(t_childproc *child)
{
	ft_close_fds(child);
	if (waitpid(child->ids[child->nr_of_cmds - 1], &child->status, 0) == -1)
		perror("minishell: ");
	if (WIFEXITED(child->status))
		child->exitcode = WEXITSTATUS(child->status); // this must be exit value
	else
		child->exitcode = -1; // is this needed?
	free(child->ids);
	child->ids = NULL;
	ft_free_pipes(child->pipes, child->pipe_count);
}

static void	ft_child_process(t_childproc *child)
{
	if ((child->i == 0 && child->nr_of_cmds > 1) || \
		(child->i != (child->nr_of_cmds - 1)))
	{
		if (dup2(child->pipes[child->i][1], STDOUT_FILENO) == -1)
			ft_throw_error(errno, "minishell: "); // free all and exit
	}
	if (child->i != 0)
	{
		if (dup2(child->pipes[child->i - 1][0], STDIN_FILENO) == -1)
			ft_throw_error(errno, "minishell: "); // free all and exit
	}
	ft_close_fds(child);
	ft_execute(child->temp->data);
}

static bool	ft_prepare_pipes(t_childproc *child, t_list *lst)
{
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

void	ft_create_child(t_list *lst) // it broke :c
{
	t_childproc	child;

	if (!ft_prepare_pipes(&child, lst) || !ft_store_output(&child))
		return ;
	while (lst)
	{
		ft_check_for_files(&child, lst->content); // make sure this checks if all infiles exist & make all outfiles, write to last one only.
		child.ids[child.i] = fork();
		if (child.ids[child.i] == -1)
		{
			perror("minishell: ");
			break ;
		}
		if (child.ids[child.i] == 0)
			ft_child_process(&child);
		child.i++;
		lst = lst->next;
	}
	if (child.oldout != -1)
		ft_restore_output(&child);
	ft_wait_for_last_child(&child);
}

// void	ft_create_child(t_list *lst)
// {
// 	t_childproc	child;
//
// 	if (!ft_prepare_pipes(&child, lst))
// 		return ;
// 	while (lst && child.i <= child.nr_of_cmds)
// 	{
// 		child.temp = lst->content;
// 		if (child.temp->type == CMD)
// 		{
// 			child.ids[child.i] = fork();
// 			if (child.ids[child.i] == -1)
// 			{
// 				perror("minishell: ");
// 				break ;
// 			}
// 			if (child.ids[child.i] == 0)
// 				ft_child_process(&child);
// 			child.i++;
// 		}
// 		lst = lst->next;
// 	}
// 	if (child.oldout != -1)
// 		ft_restore_output(&child);
// 	ft_wait_for_last_child(&child);
// }
