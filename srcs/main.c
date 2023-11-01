/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/11/01 15:55:43 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_loop(t_list *lst, t_init *process)
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
		lst = parse(process->env, process, str);
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
	process.errorcode = 0;
	if (!ft_copy_env(&process, &env, envp))
		return (process.errorcode);
	process.env = &env;
	ft_loop(&lst, &process);
	ft_free_str_array(env.new_env, NULL);
	return (process.errorcode);
}

// michelle: freeing, testing
// chey: heredoc expanding, testing.