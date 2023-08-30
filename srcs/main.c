/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 17:02:44 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/30 15:29:24 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_single_scmd(t_list *cmdlist, t_init *process)
{
	t_scmd_list	*scmd;
	t_cmd		*cmd;

	scmd = cmdlist->content;
	while (scmd)
	{
		if (scmd->type == CMD)
		{
			cmd = scmd->data;
			if (cmd->builtin == true)
			{
				ft_check_for_files(process, scmd);
				ft_run_builtin(scmd->data);
			}
			else
				ft_create_child(cmdlist, process);
		}
		scmd = scmd->next;
	}
}

void	ft_try_paths(char **path, t_cmd *cmd)
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

void	ft_find_path(t_list *cmdlist)
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
				}
			}
			tempscmd = tempscmd->next;
		}
		temp = temp->next;
	}
}

void	ft_executor(t_list *cmdlist, t_init *process)
{
	ft_find_path(cmdlist);
	if (!ft_prep(process, cmdlist) || !ft_store_old_fd(process))
	{
		ft_putendl_fd("Something went wrong, exiting..", STDERR_FILENO);
		exit(1); // or how else?
	}
	if (!cmdlist->next)
		ft_single_scmd(cmdlist, process); //something gets freed but doesnt need freeing?
	else
		ft_create_child(cmdlist, process);
	if (process->oldout != -1 || process->oldin != -1)
		ft_restore_old_fd(process);
}

int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_list	*cmdlist;
	t_env	env;
	t_init	process;

	(void)argv;
	(void)argc;
	ft_copy_env(&env, envp);
	// ft_test_signals();
	while (1)
	{
		str = readline("BabyBash: ");
		if (!str)
			break ;
		cmdlist = parse(&env, str);
		add_history(str);
		if (!cmdlist)
			continue ;
		ft_executor(cmdlist, &process);
		free(str);
		str = NULL;
	}
	ft_free_all(cmdlist, &env);
	return (EXIT_SUCCESS);
}

// to do: signals, freeing everything, expander, history, testing.
// system("leaks -q minishell_test");