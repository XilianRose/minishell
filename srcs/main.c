/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 13:37:33 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// test example: ./minishell_test infile "cat /dev/urandom" "head -n 5" cat cat outfile
// copy env, make history, make tokens, expand tokens, parse, execute, handle signals, repeat?
int	main(int argc, char **argv, char **envp)
{
	t_env	env;
	t_list	*cmdlist;
	char	*str;

	(void)argv;
	if (argc != 1)
		return (1);
	ft_copy_env(&env, envp);
	// ft_test_signals();
	while (1)
	{
		str = readline("BabyBash: ");
		puts(str);
		cmdlist = parse(&env, str);
		ft_test_child(cmdlist);
		free(str);
		str = NULL;
	}
	ft_free_env(env.new_env, NULL);
	// system("leaks -q minishell_test");
	return (EXIT_SUCCESS);
}
