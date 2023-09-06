/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/25 14:20:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/06 19:54:43 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_interactive_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
	if (signum == SIGQUIT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	ft_noninteractive_handler(int signum)
{
	if (signum == SIGINT)
		ft_putchar_fd('\n', STDERR_FILENO);
	if (signum == SIGQUIT)
		ft_putendl_fd("Quit", STDIN_FILENO);
}

void	ft_setup_noninteractive(void)
{
	struct sigaction	sa;

	sa.sa_handler = &ft_noninteractive_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	ft_setup_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = &ft_interactive_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
