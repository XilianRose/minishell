/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   childprocess.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 13:32:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 18:20:08 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_store_old_fd(t_process *child)
{
	child->oldout = dup(STDOUT_FILENO);
	if (child->oldout == -1)
	{
		ft_putstr_fd("BabyBash: old fd dup failed.", STDERR_FILENO);
		return (false);
	}
	child->oldin = dup(STDIN_FILENO);
	if (child->oldin == -1)
	{
		ft_putstr_fd("BabyBash: old fd dup failed.", STDERR_FILENO);
		return (false);
	}
	return (true);
}

static void	ft_wait_for_last_child(t_process *child)
{
	ft_close_fds(child);
	waitpid(child->ids[child->nr_of_cmds - 1], &child->status, 0);
	if (WIFEXITED(child->status))
		child->errorcode = WEXITSTATUS(child->status); // this must be exit value
	else
		child->errorcode = -1; // is this needed?
	free(child->ids);
	child->ids = NULL;
	ft_free_pipes(child->pipes, child->pipe_count);
}

void	ft_run_builtin(t_cmd *cmd)
{
	if (!ft_strncmp("echo", cmd->arg[0], 5))
		ft_echo_builtin(cmd);
	if (!ft_strncmp("cd", cmd->arg[0], 3))
		ft_cd_builtin(cmd);
	if (!ft_strncmp("pwd", cmd->arg[0], 4))
		ft_pwd_builtin();
	if (!ft_strncmp("export", cmd->arg[0], 7))
		ft_export_builtin(cmd);
	if (!ft_strncmp("unset", cmd->arg[0], 6))
		ft_unset_builtin(cmd);
	if (!ft_strncmp("env", cmd->arg[0], 4))
		ft_env_builtin(cmd);
	if (!ft_strncmp("exit", cmd->arg[0], 5))
		ft_exit_builtin(cmd);
}

static void	ft_child_process(t_process *child)
{
	if ((child->i == 0 && child->nr_of_cmds > 1) || \
		(child->i != (child->nr_of_cmds - 1)))
	{
		if (dup2(child->pipes[child->i][1], STDOUT_FILENO) == -1)
			ft_throw_error(errno, "BabyBash: "); // free all and exit
	}
	if (child->i != 0)
	{
		if (dup2(child->pipes[child->i - 1][0], STDIN_FILENO) == -1)
			ft_throw_error(errno, "BabyBash: "); // free all and exit
	}
	ft_close_fds(child);
	if (child->cmd->builtin == false)
		ft_execve(child->cmd);
	else
		ft_run_builtin(child->cmd);
}

static void	ft_find_cmd(t_process *child, t_scmd_list *lst)
{
	while (lst)
	{
		if (lst->type == CMD)
		{
			child->cmd = lst->data;
			break ;
		}
		lst = lst->next;
	}
	if (!lst)
		child->cmd = NULL;
}

void	ft_create_child(t_list *lst)
{
	t_process	child;

	if (!ft_prep(&child, lst) || !ft_store_old_fd(&child))
		return ;
	while (lst)
	{
		ft_find_cmd(&child, lst->content);
		ft_check_for_files(&child, lst->content); // make sure this checks if all infiles exist & make all outfiles, write to last one only.
		if (child.cmd && !child.errorcode)
		{
			child.ids[child.i] = fork();
			if (child.ids[child.i] == -1)
			{
				perror("BabyBash: ");
				break ;
			}
			if (child.ids[child.i] == 0)
				ft_child_process(&child);
			child.i++;
		}
		lst = lst->next;
	}
	if (child.oldout != -1 || child.oldin != -1)
		ft_restore_old_fd(&child);
	ft_wait_for_last_child(&child);
}
