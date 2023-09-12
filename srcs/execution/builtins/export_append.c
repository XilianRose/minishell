/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_append.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 18:11:54 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/12 12:07:59 by cheyennesch   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_append_to_var(t_init *process, t_export *exp, char *arg, \
	int32_t j)
{
	int32_t	add_from_pos;

	add_from_pos = ft_find_value(arg);
	exp->arg_len = ft_strlen(process->cmd->env->new_env[j]);
	exp->var_len = exp->arg_len + ft_strlen(&arg[add_from_pos]);
	exp->new_var = ft_strjoin(process->cmd->env->new_env[j], &arg[exp->name_len + 1]);
	if (!exp->new_var)
	{
		ft_throw_error(process, ENOMEM);
		return ;
	}
	free(process->cmd->env->new_env[j]);
	process->cmd->env->new_env[j] = NULL;
	process->cmd->env->new_env[j] = exp->new_var;
}

void	ft_export_append(t_init *process, t_cmd *cmd, char *arg, int32_t j)
{
	t_export	exp;

	exp.name_len = j;
	j = 0;
	while (cmd->env->new_env[j])
	{
		if (!ft_cmpname(cmd->env->new_env[j], arg, exp.name_len))
		{
			ft_check_for_plus(arg);
			if (!cmd->env->new_env[j][exp.name_len])
			{
				ft_overwrite_var(process, cmd, arg, j);
				return ;
			}
			ft_append_to_var(process, &exp, arg, j);
			return ;
		}
		j++;
	}
	ft_add_new_var(process, cmd, cmd->env, arg);
}
