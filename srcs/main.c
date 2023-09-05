/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/05 17:33:57 by cheyennesch   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_leaks(void)
{
	system("leaks -q minishell");
}

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_list	*lst;
	t_env	env;
	t_init	process;

	(void)argv, (void)argc;
	// atexit(ft_leaks);
	ft_copy_env(&env, envp);
	// ft_test_signals();
	while (1)
	{
		str = readline("BabyBash$ ");
		if (!str)
			break ;
		add_history(str);
		lst = parse(&env, str);
		free(str);
		str = NULL;
		if (!lst)
			continue ;
		ft_executor(lst, &process);
	}
	rl_clear_history();
	ft_free_str_array(env.new_env, NULL);
	return (EXIT_SUCCESS);
}

// free in parse: only free own allocated tokens etc that isn't send to executor
// to do: signals, freeing, expander, testing.