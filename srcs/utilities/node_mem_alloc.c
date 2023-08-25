/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node_mem_alloc.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/18 15:29:21 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/25 16:10:18 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_rdr	*allocate_mem_redirect(char *data, \
	t_redirect_type type)
{
	t_rdr	*redirect;

	redirect = (t_rdr *)malloc(sizeof(t_rdr));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->data = data;
	return (redirect);
}

t_cmd	*allocate_mem_cmd_info(char **arg, t_env *env, bool builtin)
{
	t_cmd	*exec;

	exec = (t_cmd *)malloc(sizeof(t_cmd));
	if (!exec)
		return (NULL);
	exec->builtin = builtin;
	exec->path = NULL;
	exec->arg = arg;
	exec->env = env;
	return (exec);
}
