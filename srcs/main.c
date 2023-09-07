/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/07 12:48:08 by cheyennesch   ########   odam.nl         */
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
		ft_setup_int32_teractive();
		str = readline("BabyBash$ ");
		str = complete_input(str);
		if (!str)
		{
			ft_putendl_fd("Exit", STDERR_FILENO);
			break ;
		}
		ft_setup_nonint32_teractive();
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

int32_t	main(int32_t argc, char **argv, char **envp)
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