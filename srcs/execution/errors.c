/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   errors.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 13:31:37 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/30 17:04:40 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_all(t_list *cmdlist, t_env *env)
{
	t_list	*temp;

	temp = cmdlist;
	while (temp)
	{
		ft_freescmdlst(temp->content);
		temp = temp->next;
	}
	ft_freelst(cmdlist);
	ft_free_env(env->new_env, NULL);
	rl_clear_history();
	// free all malloced things: env, data in nodes,
	// nodes from list, list itself, pipes, child->ids, history
}

void	ft_error_export_unset(char *name, char *option)
{
	ft_putstr_fd("BabyBash: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putchar_fd('`', STDERR_FILENO);
	ft_putstr_fd(option, STDERR_FILENO);
	ft_putchar_fd('\'', STDERR_FILENO);
	ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
}

void	ft_error_exit(char *str)
{
	ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	rl_clear_history();
	exit(255);
}

void ft_error_env(int errnr, char *str)
{
	char	*newstr;

	newstr = ft_strjoin(": ", str);
	errno = errnr;
	perror(newstr);
	free(newstr);
}

void	ft_throw_error(int errnr, char *str)
{
	errno = errnr;
	perror(str);
	rl_clear_history();
	exit(1);
}
