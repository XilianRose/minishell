/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node_mem_alloc.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/18 15:29:21 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 11:57:49 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_rdr	*allocate_mem_rdr(char *data, t_rdr_type type)
{
	t_rdr	*rdr;

	rdr = (t_rdr *)malloc(sizeof(t_rdr));
	if (!rdr)
		return (NULL);
	rdr->type = type;
	rdr->data = data;
	return (rdr);
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
