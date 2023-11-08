/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:24:50 by cschabra      #+#    #+#                 */
/*   Updated: 2023/11/08 16:01:07 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_scmd_list	*init_cmdstruct(t_list *tokens, size_t count, t_env *env)
{
	t_token	*token;
	t_cmd	*cmd;
	size_t	i;
	char	**data;
	bool	builtin;

	builtin = is_builtin(&tokens);
	data = ft_calloc(count + 1, sizeof(char *));
	if (!data)
		return (NULL);
	i = 0;
	while (i < count && tokens != NULL)
	{
		token = tokens->content;
		if (token->type == CMD_TOKEN)
			data[i++] = token->data;
		else if (token->type == RDR_TOKEN && \
		((t_token *)(tokens->next->content))->type != RDR_TOKEN)
			tokens = tokens->next;
		if (tokens != NULL)
			tokens = tokens->next;
	}
	cmd = ft_allocate_mem_cmd(data, env, builtin);
	return (ft_lstnewscmd(cmd, CMD));
}

static t_scmd_list	*init_rdrstruct(t_list *tokens)
{
	t_rdr	*rdr;
	t_token	*token;
	t_token	*next_token;

	rdr = NULL;
	token = tokens->content;
	if (tokens->next)
		next_token = tokens->next->content;
	if (tokens->next == NULL)
		return (ft_putstr_fd("BabyBash: syntax error near unexpected token\n", \
		2), free(token->data), NULL);
	if (next_token->type != CMD_TOKEN)
		return (free(token->data), NULL);
	if (ft_strncmp(token->data, ">>", 3) == 0)
		rdr = ft_allocate_mem_rdr(next_token->data, RDR_APPEND);
	else if (ft_strncmp(token->data, "<<", 3) == 0)
		rdr = ft_allocate_mem_rdr(next_token->data, HERE_DOC);
	else if (ft_strncmp(token->data, ">", 2) == 0)
		rdr = ft_allocate_mem_rdr(next_token->data, RDR_OUTPUT);
	else if (ft_strncmp(token->data, "<", 2) == 0)
		rdr = ft_allocate_mem_rdr(next_token->data, RDR_INPUT);
	else
		return (ft_putstr_fd("BabyBash: syntax error near unexpected token\n", \
		2), free(token->data), NULL);
	return (free(token->data), ft_lstnewscmd(rdr, RDR));
}

static t_list	*make_scmdlist(t_list *tokens, t_scmd_list **scmds, t_env *env, \
				size_t count)
{
	t_scmd_list	*node;

	while (tokens != NULL && ((t_token *)(tokens->content))->type != PIPE_TOKEN)
	{
		if (((t_token *)(tokens->content))->type == CMD_TOKEN && count == 0)
		{
			count = count_cmdtokens(&tokens);
			node = init_cmdstruct(tokens, count, env);
			if (node)
				scmdlst_add_back(scmds, node);
			while (tokens && ((t_token *)(tokens->content))->type == CMD_TOKEN)
				tokens = tokens->next;
		}
		else if (((t_token *)(tokens->content))->type == CMD_TOKEN && count > 0)
			tokens = tokens->next;
		else if (((t_token *)(tokens->content))->type == RDR_TOKEN)
		{
			node = init_rdrstruct(tokens);
			if (node)
				tokens = tokens->next;
			// what if init_rdr fails and has syntax error?
			tokens = tokens->next;
			scmdlst_add_back(scmds, node);
		}
	}
	return (tokens);
}

static t_list	*make_cmdlist(t_list *tokens, t_list **cmds, t_env *env)
{
	t_list		*node;
	t_scmd_list	*scmds;

	node = NULL;
	scmds = NULL;
	while (tokens)
	{
		tokens = make_scmdlist(tokens, &scmds, env, 0);
		node = ft_lstnew(scmds);
		scmds = NULL;
		if (!node)
			return (ft_lstclear(cmds, &free), NULL);
		ft_lstadd_back(cmds, node);
		if (tokens == NULL)
			return (*cmds);
		tokens = tokens->next;
	}
	return (*cmds);
}

t_list	*parse(t_env *env, t_init *process, const char *user_input)
{
	t_list	*tokens;
	t_list	*cmds;

	tokens = NULL;
	tokens = tokenisation(user_input);
	if (!tokens)
		return (ft_throw_error(process, ENOMEM), NULL);
	expand(tokens, env, process);
	remove_quotes(tokens);
	cmds = NULL;
	cmds = make_cmdlist(tokens, &cmds, env);
	free_tokenlst(tokens);
	return (cmds);
}
