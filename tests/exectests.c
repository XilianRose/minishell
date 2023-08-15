/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exectests.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/26 18:40:55 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/15 16:56:57 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_scmd_list	*ft_lstnewscmd(void *data, t_struct_type type)
{
	t_scmd_list	*newnode;

	newnode = malloc(sizeof(t_scmd_list));
	if (!newnode)
		return (NULL);
	newnode->data = data;
	newnode->type = type;
	newnode->next = NULL;
	return (newnode);
}

// linked list containing all info from 1st cmd and redirections in 1 node, if pipe 2nd cmd in second node, etc..
// node00 could be  [< infile]->[< infile]->[< infile]->[cmd1]->[> outfile]->NULL
void	ft_test_child(t_env *env)
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
	cmd1 = ft_split("cat /dev/urandom", ' ');
	cmd2 = ft_split("head -n 5", ' ');
	cmd3 = ft_split("cat", ' ');
	cmd4 = ft_split("cat", ' ');
	infile = allocate_mem_redirect(infile, "infile", RDR_INPUT);
	data1 = allocate_mem_cmd_info(data1, "/bin/cat", cmd1, env);
	data2 = allocate_mem_cmd_info(data2, "/usr/bin/head", cmd2, env);
	data3 = allocate_mem_cmd_info(data3, "/bin/cat", cmd3, env);
	data4 = allocate_mem_cmd_info(data4, "/bin/cat", cmd4, env);
	outfile = allocate_mem_redirect(outfile, "outfile", RDR_OUTPUT);

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
// 	int		i;

// 	argc = 0;
// 	argv = 0;
// 	i = 0;
// 	// atexit(ft_leaks);
// 	ft_copy_env(&env, envp);
// 	ft_test_child(&env);
// 	if (env.new_env)
// 	{
// 		if (env.new_env[i])
// 		{
// 			while (env.new_env[i])
// 				free(env.new_env[i++]);
// 		}
// 		free(env.new_env);
// 	}
// 	// while (1);
// }

// All the linked lists and their nodes will be malloced, including env. 
// Those should probably be freed at the end. Everything else free after use.