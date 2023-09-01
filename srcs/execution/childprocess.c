/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   childprocess.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 13:32:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/01 18:31:54 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_wait_for_last_child(t_init *process)
{
	ft_close_fds(process);
	waitpid(process->ids[process->nr_of_cmds - 1], &process->status, 0);
	if (WIFEXITED(process->status))
		process->errorcode = WEXITSTATUS(process->status); // this must be exit value
	else
		process->errorcode = -1; // is this needed?
	free(process->ids);
	process->ids = NULL;
	ft_free_pipes(process->pipes, process->pipe_count);
}

static void	ft_child_process(t_init *process)
{
	if ((process->i == 0 && process->nr_of_cmds > 1) || \
		(process->i != (process->nr_of_cmds - 1)))
	{
		if (dup2(process->pipes[process->i][1], STDOUT_FILENO) == -1)
			ft_throw_error(errno, "BabyBash");
	}
	if (process->i != 0 && process->heredoc == false)
	{
		if (dup2(process->pipes[process->i - 1][0], STDIN_FILENO) == -1)
			ft_throw_error(errno, "BabyBash");
	}
	ft_close_fds(process);
	if (process->cmd->builtin == false)
		ft_execve(process->cmd);
	else
	{
		ft_run_builtin(process->cmd);
		exit(0);
	}
}

static void	ft_find_cmd(t_init *process, t_scmd_list *lst)
{
	while (lst)
	{
		if (lst->type == CMD)
		{
			process->cmd = lst->data;
			break ;
		}
		lst = lst->next;
	}
	if (!lst)
		process->cmd = NULL;
}

void	ft_create_child(t_list *lst, t_init *process)
{
	while (lst)
	{
		ft_find_cmd(process, lst->content);
		ft_check_for_files(process, lst->content); // make sure this checks if all infiles exist & make all outfiles, write to last one only.
		if (process->cmd && !process->errorcode)
		{
			process->ids[process->i] = fork();
			if (process->ids[process->i] == -1)
			{
				perror("BabyBash");
				break ;
			}
			if (process->ids[process->i] == 0)
				ft_child_process(process);
			process->i++;
		}
		lst = lst->next;
	}
	ft_wait_for_last_child(process);
}
