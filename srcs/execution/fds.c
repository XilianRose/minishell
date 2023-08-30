/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fds.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 12:39:09 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/30 18:28:43 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_fds(t_init *process)
{
	int	i;

	i = 0;
	while (i < process->nr_of_cmds - 1)
	{
		if (close(process->pipes[i][0]) == -1 || close(process->pipes[i][1]) == -1)
			perror("BabyBash: "); // free all and exit
		i++;
	}
}

bool	ft_infile(t_init *process, t_rdr *rdr)
{
	process->fdin = open(rdr->data, O_RDONLY);
	if (process->fdin == -1 || dup2(process->fdin, STDIN_FILENO) == -1 || \
		close(process->fdin) == -1)
	{
		process->errorcode = errno;
		ft_putstr_fd("BabyBash: ", STDERR_FILENO);
		return (perror(rdr->data), false);
	}
	return (true);
}

bool	ft_outfile(t_init *process, t_rdr *rdr)
{
	if (rdr->type == RDR_APPEND)
		process->fdout = open(rdr->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		process->fdout = open(rdr->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (process->fdout == -1 || dup2(process->fdout, STDOUT_FILENO) == -1 || \
		close(process->fdout) == -1)
	{
		ft_putstr_fd("BabyBash: ", STDERR_FILENO);
		return (perror(rdr->data), false);
	}
	return (true);
}

void	ft_check_for_files(t_init *process, t_scmd_list *lst)
{
	t_rdr	*rdr;

	while (lst)
	{
		if (lst->type == RDR)
		{
			rdr = lst->data;
			if (rdr->type == RDR_INPUT)
			{
				if (!ft_infile(process, rdr))
					break ;
			}
			if (rdr->type == RDR_OUTPUT || rdr->type == RDR_APPEND)
				ft_outfile(process, rdr);
			if (rdr->type == HERE_DOC)
				ft_heredoc(rdr->data);
		}
		lst = lst->next;
	}
}
// bash checks all for existing, if not existing infile, error, dont check the rest.
// if all infiles exist, only read out of last one. if all outfiles are created or existing, only send to last one.