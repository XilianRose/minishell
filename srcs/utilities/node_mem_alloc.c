/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node_mem_alloc.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/18 15:29:21 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/30 17:16:27 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_rdr	*ft_allocate_mem_rdr(char *data, t_rdr_type type)
{
	t_rdr	*rdr;

	rdr = (t_rdr *)malloc(sizeof(t_rdr));
	if (!rdr)
		return (NULL);
	rdr->data = data;
	rdr->type = type;
	return (rdr);
}

t_cmd	*ft_allocate_mem_cmd(char **arg, t_env *env, bool builtin)
{
	t_cmd	*exec;

	exec = (t_cmd *)malloc(sizeof(t_cmd));
	if (!exec)
		return (NULL);
	exec->arg = arg;
	exec->env = env;
	exec->builtin = builtin;
	exec->path = NULL;
	return (exec);
}
