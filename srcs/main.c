/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/06 19:54:29 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_leaks(void)
{
	system("leaks -q minishell");
}

static void	ft_loop(t_init *process, t_env *env)
{
	char	*str;
	t_list	*lst;

	while (1)
	{
		ft_setup_interactive();
		str = readline("BabyBash$ ");
		str = complete_input(str);
		if (!str)
		{
			ft_putendl_fd("Exit", STDERR_FILENO);
			break ;
		}
		ft_setup_noninteractive();
		add_history(str);
		lst = parse(env, str);
		free(str);
		str = NULL;
		if (!lst)
			continue ;
		ft_executor(lst, process);
	}
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	// struct termios	term;
	// struct termios	original;
	t_env			env;
	t_init			process;

	(void)argv, (void)argc;
	// atexit(ft_leaks);
	// tcgetattr(STDIN_FILENO, &original);
	// tcgetattr(STDIN_FILENO, &term);
	// term.c_lflag &= ~ECHOCTL;
	// tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
	ft_copy_env(&env, envp);
	ft_loop(&process, &env);
	ft_free_str_array(env.new_env, NULL);
	// tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
	return (EXIT_SUCCESS);
}

// free in parse: only free own allocated tokens etc that isn't send to executor
// to do: signals, freeing, expander, testing.