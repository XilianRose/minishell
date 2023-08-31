/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   childutils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 16:49:24 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/31 16:56:46 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_restore_old_fd(t_init *process)
{
	if (process->oldout != -1)
	{
		if (dup2(process->oldout, STDOUT_FILENO) == -1 || \
			close (process->oldout) == -1)
			perror("BabyBash");
	}
	if (process->oldin != -1)
	{
		if (dup2(process->oldin, STDIN_FILENO) == -1 || \
			close (process->oldin) == -1)
			perror("BabyBash");
	}
}

bool	ft_store_old_fd(t_init *process)
{
	process->oldout = dup(STDOUT_FILENO);
	if (process->oldout == -1)
	{
		ft_putstr_fd("BabyBash: old fd dup failed.", STDERR_FILENO);
		return (false);
	}
	process->oldin = dup(STDIN_FILENO);
	if (process->oldin == -1)
	{
		ft_putstr_fd("BabyBash: old fd dup failed.", STDERR_FILENO);
		return (false);
	}
	return (true);
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

bool	ft_prep(t_init *process, t_list *lst)
{
	process->errorcode = 0;
	process->oldout = -1;
	process->oldin = -1;
	process->pipes = NULL;
	process->i = 0;
	process->pipe_count = ft_count_pipes(lst);
	process->nr_of_cmds = process->pipe_count + 1;
	process->ids = malloc(process->nr_of_cmds * sizeof(pid_t));
	if (!process->ids)
	{
		// free all
		ft_throw_error(errno, "BabyBash");
	}
	if (process->nr_of_cmds > 1)
		ft_create_pipes(process, process->pipe_count);
	return (true);
}
