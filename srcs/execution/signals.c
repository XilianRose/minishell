/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/25 14:20:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/01 12:00:41 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_exit(int signal)
{
	if (signal == SIGINT)
		exit(0); // dont exit with 0, exit with exitstatus
	else if (signal == SIGTSTP)
		exit(1); // handle suspending to bg here
}

void	ft_test_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = &test_exit;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	// sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		puts("in da loop");
		sleep(1);
	}
}
