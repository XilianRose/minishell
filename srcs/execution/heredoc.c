/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/04 14:59:07 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/15 13:08:53 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_heredoc(void)
{
	return ;
	// open pipe to write into
	// while (1)
	//	readline (">")
	// if strcmp (delimiter)
	// break
	// check for empty heredoc, if delimiter is instantly given.
	// only if the delimiter is not between quotes. cat << EOF or cat << ";" / cat << ';' work differently.
	// ";" or ';' expansion doesn't happen. check conv with leon for quotes.
	// handle expansion if found.
	// write into pipe.
	// dup to make read end the stdin.
}
// what i need for heredoc: dont need the cmd. this can be run like normal commands.
// check for redirections in childprocesses themselves, currently only works for first cmd or first found >/>>.