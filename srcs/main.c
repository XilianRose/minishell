/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 16:23:16 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_single_scmd(void)
{
	// if 1 builtin send to right function, if not make child to exec.
	return ;
}

static void	ft_try_paths(char **path, t_cmd *cmd)
{
	int32_t	i;
	char	*cmdpath;
	char	*temp;

	i = 0;
	while (path[i])
	{
		temp = ft_strjoin(path[i], "/");
		if (!temp)
			ft_putendl_fd("ft_try_paths strjoin error", STDERR_FILENO);
		cmdpath = ft_strjoin(temp, cmd->arg[0]);
		if (!cmdpath)
			ft_putendl_fd("ft_try_paths strjoin error", STDERR_FILENO);
		free(temp);
		if (access(cmdpath, F_OK) == 0)
			cmd->path = cmdpath;
		i++;
	}
}

static void	ft_find_path(t_list *cmdlist)
{
	t_list		*temp;
	t_scmd_list	*tempscmd;
	t_cmd		*tempcmd;
	int			i;
	char		**path;

	i = 0;
	temp = cmdlist;
	while (temp)
	{
		tempscmd = temp->content;
		while (tempscmd)
		{
			if (tempscmd->type == CMD)
			{
				tempcmd = tempscmd->data;
				if (tempcmd->builtin == false)
				{
					while (tempcmd->env->new_env[i])
					{
						if (ft_strncmp(tempcmd->env->new_env[i], "PATH=", 5) == 0)
						{
							path = ft_split(tempcmd->env->new_env[i] + 5, ':');
							if (!path)
								ft_putendl_fd("ft_find_paths split error", STDERR_FILENO);
						}
						i++;
					}
					ft_try_paths(path, tempcmd);
					puts(tempcmd->path);
				}
			}
			tempscmd = tempscmd->next;
		}
		temp = temp->next;
	}
}

// test example: ./minishell_test infile "cat /dev/urandom" "head -n 5" cat cat outfile
// copy env, make history, make tokens, expand tokens, parse, execute, handle signals, repeat?
int	main(int argc, char **argv, char **envp)
{
	t_env	env;
	t_list	*cmdlist;
	t_list	*temp;
	char	*str;

	(void)argv;
	if (argc != 1)
		return (1);
	ft_copy_env(&env, envp);
	// ft_test_signals();
	while (1)
	{
		str = readline("BabyBash: ");
		cmdlist = parse(&env, str);
		if (!cmdlist)
			continue ;
		ft_find_path(cmdlist);
		if (!cmdlist->next)
			ft_single_scmd();
		else
			ft_create_child(cmdlist);
	}
	free(str);
	str = NULL;
	temp = cmdlist;
	while (temp)
	{
		ft_freescmdlst(temp->content);
		temp = temp->next;
	}
	ft_freelst(cmdlist);
	ft_free_env(env.new_env, NULL);
	// system("leaks -q minishell_test");
	return (EXIT_SUCCESS);
}
