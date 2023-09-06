/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/06 14:15:15 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_commandlist(t_list *lst)
{
	t_scmd_list	*scmd;
	t_cmd		*cmd;
	t_rdr		*rdr;
	int			i;
	int			j;

	i = 1;
	while (lst != NULL)
	{
		printf("______________________________________________________________________\n");
		printf("scmd #%i\n", i);
		scmd = lst->content;
		while (scmd != NULL)
		{
			printf("node type:	[%d]\n", scmd->type);
			if (scmd->type == CMD)
			{
				cmd = scmd->data;
				printf("path:		%s\n", cmd->path);
				j = 0;
				while (cmd->arg[j] != NULL)
				{
					printf("args:		%s\n", cmd->arg[j]);
					j++;
				}
				printf("builtin:	%d\n\n", cmd->builtin);
			}
			else if (scmd->type == RDR)
			{
				rdr = scmd->data;
				printf("data:		%s\n", rdr->data);
				printf("rdr type:	%d\n\n", rdr->type);
			}
			scmd = scmd->next;
		}
		lst = lst->next;
		i++;
	}
}

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
		str = complete_input(str);
		if (!str)
			break ;
		add_history(str);
		lst = parse(&env, str);
		free(str);
		str = NULL;
		if (!lst)
			continue ;
		ft_executor(lst, &process);
		print_commandlist(lst);
		free(str);
		str = NULL;
	}
	rl_clear_history();
	ft_free_str_array(env.new_env, NULL);
	return (EXIT_SUCCESS);
}

// free in parse: only free own allocated tokens etc that isn't send to executor
// to do: signals, freeing, expander, testing.