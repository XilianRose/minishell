/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:24:50 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/24 18:07:40 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_scmd_list	*make_rdrnode(t_list *tokens, t_scmd_list **scmds)
{
	t_scmd_list		*node;
	t_rdr			*rdr;
	t_token			*token;

	rdr = allocate_mem_redirect(rdr, )
	//how to allocate rdr struct?
	while (tokens->next != NULL && tokens->next != '|')
	{
		token = tokens->content;
		if (token->type == REDIRECTION_TOKEN)
		{
			if (ft_strncmp(token->data, ">", 2) == 0)
				rdr->type = RDR_OUTPUT;
				// rdr = allocate_mem_redirect(rdr, tokens.next.content.data, rdr_ouput)
			if (ft_strncmp(token->data, "<", 2) == 0)
				rdr->type = RDR_INPUT;
			if (ft_strncmp(token->data, ">>", 3) == 0)
				rdr->type = RDR_APPEND;
			if (ft_strncmp(token->data, "<<", 3) == 0)
				rdr->type = HERE_DOC;
		}
		else
	}

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
			has_rdr = true;
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
