/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/01 18:30:07 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief execve returns -1 if it fails and sets errno, 
 * on success it ends the process
 * 
 * @param cmd 
 */
void	ft_execve(t_cmd *cmd)
{
	if (access(cmd->path, F_OK) == -1)
	{
		ft_putstr_fd("BabyBash: ", STDERR_FILENO);
		ft_putstr_fd(cmd->arg[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}
	else
	{
		if (execve(cmd->path, cmd->arg, cmd->env->new_env) == -1)
			ft_throw_error(errno, "BabyBash");
	}
}

static void	ft_single_scmd(t_list *lst, t_init *process)
{
	t_scmd_list	*scmd;
	t_cmd		*cmd;

	scmd = lst->content;
	while (scmd)
	{
		if (!scmd->next && scmd->type == RDR)
			ft_check_for_files(process, scmd);
		else if (scmd->type == CMD)
		{
			cmd = scmd->data;
			if (cmd->builtin == true)
			{
				ft_check_for_files(process, scmd);
				ft_run_builtin(scmd->data);
				break ;
			}
			else
				ft_create_child(lst, process);
		}
		scmd = scmd->next;
	}
}

bool	ft_executor(t_list *lst, t_init *process)
{
	if (!ft_find_path(lst) || !ft_prep(process, lst) || \
		!ft_store_old_fd(process))
	{
		ft_putendl_fd("Something went wrong", STDERR_FILENO);
		return (false); // free all
	}
	if (!lst->next)
		ft_single_scmd(lst, process);
	else
		ft_create_child(lst, process);
	if (process->oldout != -1 || process->oldin != -1)
		ft_restore_old_fd(process);
	return (true);
}
