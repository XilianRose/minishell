/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:24:50 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/25 15:59:18 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_cmdtokens(t_list *tokens)
{
	t_token	*token;
	size_t	count;

	count = 0;
	while (token->type == CMD_OR_FILE_TOKEN)
	{
		token = tokens->content;
		count++;
		tokens = tokens->next;
	}
	return (count);
}

static t_cmd	*init_cmdstruct(t_list *tokens, t_env *env)
{
	t_token	*token;
	size_t	count;
	size_t	i;
	char	**data;
	t_cmd	*cmd;

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
	cmd = allocate_mem_cmd_info(cmd, NULL, data, env);
	return (cmd);
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
		return (allocate_mem_redirect(rdr, next_token->data, RDR_OUTPUT));
	else if (token->data == "<")
		return (allocate_mem_redirect(rdr, next_token->data, RDR_INPUT));
	else if (token->data == ">>")
		return (allocate_mem_redirect(rdr, next_token->data, RDR_APPEND));
	else
		return (allocate_mem_redirect(rdr, next_token->data, HERE_DOC));
}

static size_t	*make_scmdlist(t_list *tokens, t_scmd_list **scmds)
{
	t_scmd_list		*node;
	t_rdr			*rdr;
	t_cmd			*cmd;
	t_token			*token;
	bool			has_rdr;

	has_rdr = false;
	while (tokens->next != NULL && tokens->next != '|')
	{
		token = tokens->content;
		if (token->type == CMD_OR_FILE_TOKEN && has_rdr == false)
		{
			node = make_rdrnode(tokens, scmds);
		}
		if (token->type == REDIRECTION_TOKEN)
		{
			tokens = tokens->next->next;
		}
	}
	return (0);
}

static size_t	make_cmdlist(t_list *tokens, t_list **cmds)
{
	t_list		*node;
	t_scmd_list	*scmds;

	while (tokens->next != NULL)
	{
		make_scmdlist(tokens, &scmds);
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

	tokens = NULL;
	tokens = tokenisation(user_input);
	make_cmdlist(tokens, &cmds);
}
