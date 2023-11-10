/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 13:32:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/11/09 17:54:58 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_echo_builtin(t_init *process, t_cmd *cmd)
{
	int32_t	i;

	i = 1;
	while (cmd->arg[i] && str_equals("-n", cmd->arg[i]))
		i++;
	if (cmd->arg[i])
	{
		while (cmd->arg[i + 1])
		{
			if (write(STDOUT_FILENO, cmd->arg[i], ft_strlen(cmd->arg[i])) == \
				-1 || write(STDOUT_FILENO, " ", 1) == -1)
				return (ft_throw_error(process, errno), false);
			i++;
		}
		if (write(STDOUT_FILENO, cmd->arg[i], ft_strlen(cmd->arg[i])) == -1)
			return (ft_throw_error(process, errno), false);
	}
	if (cmd->arg[1] && cmd->arg[1][0])
	{
		if (str_equals("-n", cmd->arg[1]))
			return (true);
	}
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return (ft_throw_error(process, errno), false);
	return (true);
}

void	ft_cd_builtin(t_init *process, t_cmd *cmd)
{
	int32_t	i;

	i = 0;
	if (!cmd->arg[1] || str_equals("~", cmd->arg[1]))
	{
		while (cmd->env->new_env[i])
		{
			if (ft_strncmp(cmd->env->new_env[i], "HOME=", 5) == 0)
			{
				if (chdir(cmd->env->new_env[i] + 5) == -1)
				{
					ft_throw_error(process, errno);
					return ;
				}
			}
			i++;
		}
	}
	else if (chdir(cmd->arg[1]) == -1)
	{
		ft_throw_error(process, errno);
		return ;
	}
	process->errorcode = 0;
}

void	ft_pwd_builtin(t_init *process)
{
	char	buffer[MAXPATHLEN];

	if (getcwd(buffer, MAXPATHLEN) == NULL)
	{
		ft_throw_error(process, errno);
		return ;
	}
	printf("%s\n", buffer);
	process->errorcode = 0;
}

void	ft_exit_builtin(t_list *lst, t_init *process, t_cmd *cmd)
{
	int32_t	i;
	int64_t	string_to_llong;

	i = 0;
	string_to_llong = process->errorcode;
	if (cmd->arg[1])
	{
		if (cmd->arg[1][0] == '-' || cmd->arg[1][0] == '+')
			i++;
		while (cmd->arg[1][i])
		{
			if (!ft_isdigit(cmd->arg[1][i]))
				ft_error_exit(lst, process, cmd->arg[1]);
			i++;
		}
		string_to_llong = ft_atollong(lst, process, cmd->arg[1]) % 256;
		if (string_to_llong < 0)
			string_to_llong += 256;
	}
	ft_putendl_fd("exit", STDERR_FILENO);
	rl_clear_history();
	ft_free_str_array(process->env->new_env, NULL);
	ft_reset_process(lst, process);
	exit(string_to_llong);
}
