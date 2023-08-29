/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exectests.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/26 18:40:55 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 11:56:35 by cschabra      ########   odam.nl         */
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

void	ft_test_child(t_env *env, char **argv)
{
	char		**cmd1;
	char		**cmd2;
	char		**cmd3;
	char		**cmd4;
	t_list		*head;
	t_scmd_list	*cmdhead0;
	t_scmd_list	*cmdhead1;
	t_scmd_list	*cmdhead2;
	t_scmd_list	*cmdhead3;
	t_cmd		*data1;
	t_cmd		*data2;
	t_cmd		*data3;
	t_cmd		*data4;
	t_rdr		*infile;
	t_rdr		*outfile;

	infile = NULL;
	data1 = NULL;
	data2 = NULL;
	data3 = NULL;
	data4 = NULL;
	outfile = NULL;
	head = NULL;
	cmd1 = ft_split(argv[2], ' ');
	cmd2 = ft_split(argv[3], ' ');
	cmd3 = ft_split(argv[4], ' ');
	cmd4 = ft_split(argv[5], ' ');
	infile = allocate_mem_rdr(argv[1], RDR_INPUT);
	data1 = allocate_mem_cmd_info(cmd1, env, false);
	data2 = allocate_mem_cmd_info(cmd2, env, false);
	data3 = allocate_mem_cmd_info(cmd3, env, false);
	data4 = allocate_mem_cmd_info(cmd4, env, false);
	outfile = allocate_mem_rdr(argv[6], RDR_OUTPUT);
	data1->path = "/bin/cat";
	data2->path = "/usr/bin/head";
	data3->path = "/bin/cat";
	data4->path = "/bin/cat";

	cmdhead0 = ft_lstnewscmd(infile, RDR);
	cmdhead0->next = ft_lstnewscmd(data1, CMD);

	cmdhead1 = ft_lstnewscmd(data2, CMD);

	cmdhead2 = ft_lstnewscmd(data3, CMD);

	cmdhead3 = ft_lstnewscmd(data4, CMD);
	cmdhead3->next = ft_lstnewscmd(outfile, RDR);

	head = ft_lstnew(cmdhead0);
	head->next = ft_lstnew(cmdhead1);
	head->next->next = ft_lstnew(cmdhead2);
	head->next->next->next = ft_lstnew(cmdhead3);

	ft_create_child(head);
	t_list *temp;
	temp = head;
	while (temp)
	{
		ft_freescmdlst(temp->content);
		temp = temp->next;
	}
	ft_freelst(head);
	int	i = 0;
	while (cmd1[i])
		free(cmd1[i++]);
	i = 0;
	while (cmd2[i])
		free(cmd2[i++]);
	i = 0;
	while (cmd3[i])
		free(cmd3[i++]);
	i = 0;
	while (cmd4[i])
		free(cmd4[i++]);
	free(infile);
	free(outfile);
	free(cmd1);
	free(cmd2);
	free(cmd3);
	free(cmd4);
	free(data1);
	free(data2);
	free(data3);
	free(data4);
}

// void	ft_leaks(void)
// {
// 	system("leaks -q minishell_test");
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	t_env	env;

// 	argc = 0;
// 	argv = 0;
// 	// atexit(ft_leaks);
// 	ft_copy_env(&env, envp);
// 	ft_test_child(&env);
// 	ft_free_env(env.new_env, NULL);
// 	// while (1);
// }

// All the linked lists and their nodes will be malloced, including env. 
// Those should probably be freed at the end. Everything else free after use.