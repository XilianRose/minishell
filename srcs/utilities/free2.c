/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free2.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/18 12:21:06 by mstegema      #+#    #+#                 */
/*   Updated: 2023/12/18 12:29:02 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_rdrstruct(t_rdr *rdr)
{
	if (rdr->data)
	{
		free(rdr->data);
		rdr->data = NULL;
	}
	free(rdr);
	rdr = NULL;
}

void	free_cmdstruct(t_cmd *cmd)
{
	ft_free_str_array(cmd->arg, cmd->path);
	free(cmd);
	cmd = NULL;
}
