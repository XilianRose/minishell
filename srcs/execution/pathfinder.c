/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pathfinder.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/01 16:57:17 by cschabra      #+#    #+#                 */
/*   Updated: 2023/09/01 16:58:24 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	ft_find_path2(t_scmd_list *tempscmd)
{
	t_cmd	*tempcmd;
	char	**path;
	int		i;

	i = 0;

	tempcmd = tempscmd->data;
	if (tempcmd->builtin == false)
	{
		while (tempcmd->env->new_env[i])
		{
			if (ft_strncmp(tempcmd->env->new_env[i], "PATH=", 5) == 0)
			{
				path = ft_split(tempcmd->env->new_env[i] + 5, ':');
				if (!path)
					ft_putendl_fd("no", STDERR_FILENO); // free all exit
			}
			i++;
		}
		ft_try_paths(path, tempcmd);
	}
}

void	ft_find_path(t_list *lst)
{
	t_list		*temp;
	t_scmd_list	*tempscmd;

	temp = lst;
	while (temp)
	{
		tempscmd = temp->content;
		while (tempscmd)
		{
			if (tempscmd->type == CMD)
				ft_find_path2(tempscmd);
			tempscmd = tempscmd->next;
		}
		temp = temp->next;
	}
}
