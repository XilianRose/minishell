/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/11/15 16:44:04 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_reset_process(t_list *lst, t_init *process)
{
	ft_freelst(lst);
	if (process->ids)
	{
		free(process->ids);
		process->ids = NULL;
	}
	process->cmd = NULL;
	process->i = 0;
	process->nr_of_cmds = 0;
	if (process->pipes)
		ft_free_pipes(process, process->pipe_count);
	process->pipe_count = 0;
	process->fdin = 0;
	process->fdout = 0;
	if (process->oldout != -1 || process->oldin != -1)
		ft_restore_old_fd(process);
	process->heredoc = false;
}

void	ft_execve(t_list *lst, t_init *process)
{
	rl_clear_history();
	if (!process->cmd->path || access(process->cmd->path, F_OK) == -1)
	{
		ft_putstr_fd("BabyBash: ", STDERR_FILENO);
		ft_putstr_fd(process->cmd->arg[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free_str_array(process->env->new_env, NULL);
		ft_reset_process(lst, process);
		exit(127);
	}
	else
	{
		if (execve(process->cmd->path, process->cmd->arg, \
			process->cmd->env->new_env) == -1)
		{
			perror("BabyBash");
			ft_free_str_array(process->env->new_env, NULL);
			ft_reset_process(lst, process);
			exit(127);
		}
	}
}

static void	ft_single_scmd(t_list *lst, t_init *process)
{
	t_scmd_list	*scmd;

	scmd = lst->content;
	if (!ft_check_for_files(scmd, process))
		return ;
	while (scmd)
	{
		if (scmd->type == CMD)
		{
			process->cmd = scmd->data;
			if (process->cmd->builtin == true)
			{
				ft_run_builtin(lst, process, scmd->data);
				break ;
			}
			else
			{
				ft_create_child(lst, process);
				ft_wait_for_last_child(process);
			}
		}
		scmd = scmd->next;
	}
	return ;
}

static bool	ft_executor2(t_list *lst, t_init *process)
{
	if (!lst->next)
	{
		ft_single_scmd(lst, process);
		if (process->must_exit == true)
			return (false);
		return (true);
	}
	while (process->must_exit == false && lst)
	{
		if (!ft_check_for_files(lst->content, process))
			break ;
		ft_create_child(lst, process);
		ft_restore_old_fd(process);
		ft_store_old_fd(process);
		lst = lst->next;
	}
	ft_wait_for_last_child(process);
	if (process->must_exit == true)
		return (false);
	return (true);
}

bool	ft_executor(t_list *lst, t_init *process)
{
	if (!lst)
		return (true);
	if (!ft_prep(lst, process) || !ft_find_path(lst, process))
	{
		ft_reset_process(lst, process);
		process->errorcode = 1;
		ft_putendl_fd("Something went wrong in preparations..", STDERR_FILENO);
		return (false);
	}
	ft_store_old_fd(process);
	if (process->must_exit || !ft_executor2(lst, process))
	{
		ft_reset_process(lst, process);
		return (false);
	}
	ft_reset_process(lst, process);
	return (true);
}
