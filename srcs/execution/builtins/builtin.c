/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 13:32:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 16:15:33 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// one builtin doesnt create child processes, but does when pipes are used. 
// needs to be able to write into pipe too, also handle $?
void	ft_echo_builtin(t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!cmd->arg[i] || !cmd->arg[i][0])
	{
		printf("\n");
		return ;
	}
	if (cmd->arg[i] && str_equals("-n", cmd->arg[1]))
	{
		while (str_equals("-n", cmd->arg[i]))
		{
			i++;
			if (!cmd->arg[i])
				return ;
		}
		while (cmd->arg[i + 1])
			printf("%s ", cmd->arg[i++]);
		printf("%s", cmd->arg[i++]);
		return ;
	}
	while (cmd->arg[i + 1])
		printf("%s ", cmd->arg[i++]);
	printf("%s\n", cmd->arg[i]);
}

// only changes directory inside the process, which exits.
// I can print current dir with getcwd to test if it actually changed.
void	ft_cd_builtin(t_cmd *cmd)
{
	if (chdir(cmd->arg[1]) == -1)
		ft_throw_error(errno, "BabyBash: ");
}

void	ft_pwd_builtin(void)
{
	char	buffer[MAXPATHLEN];

	if (getcwd(buffer, MAXPATHLEN) == NULL)
		ft_throw_error(errno, "BabyBash: ");
	printf("%s\n", buffer);
}

// I made it so it always exits with an int between 0-255 just like bash does.
void	ft_exit_builtin(t_cmd *cmd)
{
	int				i;
	long long int	string_to_llong;

	i = 0;
	if (!cmd->arg[1])
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		exit(0);
	}
	if (cmd->arg[1][0] == '-' || cmd->arg[1][0] == '+')
		i++;
	while (cmd->arg[1][i])
	{
		if (!ft_isdigit(cmd->arg[1][i]))
			ft_error_exit(cmd->arg[1]);
		i++;
	}
	string_to_llong = ft_atollong(cmd->arg[1]) % 256;
	if (string_to_llong < 0)
		string_to_llong += 256;
	ft_putendl_fd("exit", STDERR_FILENO);
	exit(string_to_llong);
}
