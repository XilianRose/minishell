/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/07 19:27:03 by cheyennesch   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int32_t	sigint_check;

void	ft_leaks(void)
{
	system("leaks -q minishell");
}

static void	ft_loop(t_list *lst, t_init *process, t_env *env)
{
	char	*str;

	while (1)
	{
		ft_setup_interactive(process);
		str = readline("BabyBash$ ");
		if (!str)
		{
			ft_putendl_fd("Exit", STDERR_FILENO);
			break ;
		}
		str = complete_input(str);
		ft_setup_noninteractive(process);
		if (ft_strlen(str))
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
	t_list	lst;
	t_init	process;
	t_env	env;

	(void)argv, (void)argc;
	// atexit(ft_leaks);
	process.errorcode = 0;
	sigint_check = 0;
	if (!ft_copy_env(&process, &env, envp))
		return (process.errorcode);
	ft_loop(&lst, &process, &env);
	ft_free_str_array(env.new_env, NULL);
	if (sigint_check == SIGINT)
		process.errorcode = 130;
	return (process.errorcode);
}

// free in parse: only free own allocated tokens etc that isn't send to executor
// to do: wait for all children and fix errorcode/continue builtins errorcode, freeing, expander, handling quotes, testing.