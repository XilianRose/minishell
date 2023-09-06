/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/31 11:18:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/04 15:23:46 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env_builtin(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (str_equals("env", cmd->arg[0]))
	{
		if (cmd->arg[1])
		{
			ft_putstr_fd(cmd->arg[0], STDERR_FILENO);
			ft_error_env(ENOENT, cmd->arg[1]); // set exitcode to 1
			return ;
		}
		while (cmd->env->new_env[i])
		{
			if (ft_strchr(cmd->env->new_env[i], '='))
				printf("%s\n", cmd->env->new_env[i]);
			i++;
		}
	}
}

void	ft_copy_env(t_env *env, char **old_env)
{
	int		i;
	size_t	str_len;

	i = 0;
	env->env_len = 0;
	while (old_env[env->env_len])
		env->env_len++;
	env->new_env = malloc((env->env_len + 1) * sizeof(char *));
	if (!env->new_env)
		ft_throw_error(errno, "BabyBash");
	while (old_env[i])
	{
		str_len = ft_strlen(old_env[i]);
		env->new_env[i] = malloc((str_len + 1) * sizeof(char));
		if (!env->new_env[i])
		{
			ft_free_str_array(env->new_env, NULL);
			ft_throw_error(ENOMEM, "BabyBash");
		}
		ft_memcpy(env->new_env[i], old_env[i], (str_len + 1));
		i++;
	}
	env->new_env[i] = NULL;
}
