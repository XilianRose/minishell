/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_append.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/12 18:11:54 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/08 18:18:08 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_append_to_var(t_init *process, t_export *exp, char *arg, \
	int32_t j)
{
	t_cmd	*cmd;
	int32_t	add_from_pos;

	cmd = process->cmd;
	add_from_pos = ft_find_value(arg);
	exp->arg_len = ft_strlen(cmd->env->new_env[j]);
	exp->var_len = exp->arg_len + ft_strlen(&arg[add_from_pos]);
	if (exp->var_len == exp->arg_len && cmd->env->new_env[j][exp->arg_len] != '=') // broke
	{
		exp->new_var = ft_strjoin(cmd->env->new_env[j], "=");
	}
	else
		exp->new_var = ft_strjoin(cmd->env->new_env[j], &arg[exp->name_len + 2]);
	if (!exp->new_var)
		ft_throw_error(process, ENOMEM);
	free(cmd->env->new_env[j]);
	cmd->env->new_env[j] = NULL;
	cmd->env->new_env[j] = exp->new_var;
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
			ft_append_to_var(process, &exp, arg, j);
			return ;
		}
		j++;
	}
	ft_add_new_var(process, cmd, cmd->env, arg);
}
