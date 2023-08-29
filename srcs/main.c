/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 15:08:03 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	print_commandlist(t_list *lst)
// {
// 	t_scmd_list	*scmd;
// 	t_cmd		*cmd;
// 	t_rdr		*rdr;
// 	int			i;
// 	int			j;

// 	i = 1;
// 	while (lst != NULL)
// 	{
// 		printf("scmd #%i\n", i);
// 		printf("______________________________________________________________________\n");
// 		scmd = lst->content;
// 		while (scmd != NULL)
// 		{
// 			printf("node type:	[%d]\n", scmd->type);
// 			if (scmd->type == CMD)
// 			{
// 				cmd = scmd->data;
// 				printf("path:		%s\n", cmd->path);
// 				j = 0;
// 				while (cmd->arg[j] != NULL)
// 				{
// 					printf("args:		%s\n", cmd->arg[j]);
// 					j++;
// 				}
// 				printf("builtin:	%d\n\n", cmd->builtin);
// 			}
// 			else if (scmd->type == RDR)
// 			{
// 				rdr = scmd->data;
// 				printf("data:		%s\n", rdr->data);
// 				printf("rdr type:	%d\n\n", rdr->type);
// 			}
// 			scmd = scmd->next;
// 		}
// 		lst = lst->next;
// 		i++;
// 	}
// }

// // test example: ./minishell_test infile "cat /dev/urandom" "head -n 5" cat cat outfile
// // copy env, make history, make tokens, expand tokens, parse, execute, handle signals, repeat?
// int	main(int argc, char **argv, char **envp)
// {
// 	t_env	env;
// 	t_list	*cmdlist;
// 	char	*str;

// 	(void)argv;
// 	if (argc != 1)
// 		return (1);
// 	ft_copy_env(&env, envp);
// 	// ft_test_signals();
// 	while (1)
// 	{
// 		// str = readline("BabyBash: ");
// 		str = "cat";
// 		puts(str);
// 		cmdlist = parse(&env, str);
// 		// ft_test_child(cmdlist);
// 		print_commandlist(cmdlist);
// 	}
// 	free(str);
// 	str = NULL;
// 	ft_free_env(env.new_env, NULL);
// 	// system("leaks -q minishell_test");
// 	return (EXIT_SUCCESS);
// }
