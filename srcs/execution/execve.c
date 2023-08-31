/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execve.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/31 14:54:11 by cschabra      ########   odam.nl         */
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
		// free all
		exit(127);
	}
	else
	{
		if (execve(cmd->path, cmd->arg, cmd->env->new_env) == -1)
			ft_throw_error(errno, "BabyBash");
			// free all
	}
}
