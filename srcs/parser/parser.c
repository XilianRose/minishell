/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:24:50 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 15:47:59 by mstegema      ########   odam.nl         */
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
		return (NULL); //throw error
	i = 0;
	while (i < count)
	{
		token = tokens->content;
		data[i] = token->data;
		i++;
		tokens = tokens->next;
	}
	cmd = allocate_mem_cmd_info(data, env, builtin);
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
	if (tokens->next == NULL || next_token->type != CMD_OR_FILE_TOKEN)
		return (ft_putstr_fd("BabyBash: syntax error near unexpected token\n", \
		2), NULL);
	if (ft_strncmp(token->data, ">", 2) == 0)
		rdr = allocate_mem_rdr(next_token->data, RDR_OUTPUT);
	else if (ft_strncmp(token->data, "<", 2) == 0)
		rdr = allocate_mem_rdr(next_token->data, RDR_INPUT);
	else if (ft_strncmp(token->data, ">>", 3) == 0)
		rdr = allocate_mem_rdr(next_token->data, RDR_APPEND);
	else
		rdr = allocate_mem_rdr(next_token->data, HERE_DOC);
	return (ft_lstnewscmd(rdr, RDR));
}

t_list	*make_scmdlist(t_list *tokens, t_scmd_list **scmds, t_env *env)
{
	t_scmd_list		*node;
	t_token			*token;
	size_t			count;

	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == CMD_OR_FILE_TOKEN)
		{
			count = count_cmdtokens(&tokens);
			node = init_cmdstruct(tokens, count, env);
			while (count-- > 0)
				tokens = tokens->next;
		}
		else if (token->type == RDR_TOKEN)
		{
			node = init_rdrstruct(tokens);
			if (node)
				tokens = tokens->next->next;
		}
		if (!node || token->type == PIPE_TOKEN)
			return (tokens);
		scmdlst_add_back(scmds, node);
	}
	return (tokens);
}

static size_t	make_cmdlist(t_list *tokens, t_list **cmds, t_env *env)
{
	t_list		*node;
	t_scmd_list	*scmds;

	node = NULL;
	scmds = NULL;
	while (tokens)
	{
		tokens = make_scmdlist(tokens, &scmds, env);
		node = ft_lstnew(scmds);
		scmds = NULL;
		if (!node)
			return (ft_lstclear(cmds, &free), 1);
		ft_lstadd_back(cmds, node);
		if (tokens == NULL)
			return (0);
		tokens = tokens->next;
	}
	return (0);
}

t_list	*parse(t_env *env, const char *user_input)
{
	t_list	*tokens;
	t_list	*cmds;

	tokens = NULL;
	tokens = tokenisation(user_input);
	cmds = NULL;
	make_cmdlist(tokens, &cmds, env);
	return (cmds);
}
