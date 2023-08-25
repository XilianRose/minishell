/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:24:50 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/25 17:52:23 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	scmdlst_add_back(t_scmd_list **scmds, t_scmd_list *new)
{
	t_scmd_list	*temp;

	if (!new)
		return ;
	if (!*scmds)
	{
		*scmds = new;
		return ;
	}
	temp = *scmds;
	while (temp != NULL && temp -> next != NULL)
		temp = temp -> next;
	temp -> next = new;
}

size_t	count_cmdtokens(t_list **tokens)
{
	t_list	*current;
	t_token	*token;
	size_t	count;

	current = *tokens;
	count = 0;
	while (token->type == CMD_OR_FILE_TOKEN)
	{
		token = current->content;
		count++;
		current = current->next;
	}
	return (count);
}

bool	is_builtin(t_list **tokens)
{
	t_list	*current;
	t_token	*token;
	char	*str;

	current = *tokens;
	token = current->content;
	str = token->data;
	if ((ft_strncmp(str, "echo", 5) == 0) || (ft_strncmp(str, "cd", 3) == 0) \
	|| (ft_strncmp(str, "pwd", 4) == 0) || (ft_strncmp(str, "export", 7) == 0) \
	|| (ft_strncmp(str, "unset", 6) == 0) || (ft_strncmp(str, "env", 4) == 0) \
	|| (ft_strncmp(str, "exit", 5) == 0))
		return (true);
	return (false);
}

static t_cmd	*init_cmdstruct(t_list *tokens, t_env *env)
{
	t_token	*token;
	size_t	count;
	size_t	i;
	char	**data;
	bool	builtin;

	builtin = is_builtin(tokens);
	count = count_cmdtokens(tokens);
	data = ft_calloc(count + 1, sizeof(char *));
	if (!data)
		return (NULL); //throw error
	i = 0;
	while (i < count)
	{
		token = tokens->content;
		data[i] = token->data;
		i++;
		tokens = tokens->next;
	}
	return (allocate_mem_cmd_info(data, env, builtin));
}

static t_rdr	*init_rdrstruct(t_list *tokens)
{
	t_rdr	*rdr;
	t_token	*token;
	t_token	*next_token;

	rdr = NULL;
	token = tokens->content;
	next_token = tokens->next->content;
	if (next_token->type != CMD_OR_FILE_TOKEN)
		return (ft_putstr_fd("minishell: syntax error \
		near unexpected token\n", 2), NULL);
	if (token->data == ">")
		return (allocate_mem_redirect(next_token->data, RDR_OUTPUT));
	else if (token->data == "<")
		return (allocate_mem_redirect(next_token->data, RDR_INPUT));
	else if (token->data == ">>")
		return (allocate_mem_redirect(next_token->data, RDR_APPEND));
	else
		return (allocate_mem_redirect(next_token->data, HERE_DOC));
}

static size_t	*make_scmdlist(t_list *tokens, t_scmd_list **scmds, t_env *env)
{
	t_scmd_list		*node;
	t_rdr			*rdr;
	t_cmd			*cmd;
	t_token			*token;
	size_t			count;

	while (tokens->next != NULL && tokens->next != '|')
	{
		token = tokens->content;
		if (token->type == CMD_OR_FILE_TOKEN)
			node = init_cmdstruct(tokens, env);
		else if (token->type == REDIRECTION_TOKEN)
		{
			node = init_rdrstruct(tokens);
			tokens = tokens->next->next;
		}
		if (!node)
			return ;
		scmdlst_add_back(scmds, node);
		tokens = tokens->next;
	}
	return (0);
}

static size_t	make_cmdlist(t_list *tokens, t_list **cmds, t_env *env)
{
	t_list		*node;
	t_scmd_list	*scmds;

	while (tokens->next != NULL)
	{
		make_scmdlist(tokens, &scmds, env);
		node = ft_lstnew(scmds);
		if (!node)
			return (ft_lstclear(cmds, &free), 1);
	}
	return (0);
}

t_list	*parse(const char *user_input)
{
	t_list	*tokens;
	t_list	*cmds;
	t_env	*env;

	tokens = NULL;
	tokens = tokenisation(user_input);
	make_cmdlist(tokens, &cmds, env);
}
