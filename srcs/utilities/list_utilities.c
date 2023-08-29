/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_utilities.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:25:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/28 17:50:29 by mstegema      ########   odam.nl         */
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

void	ft_freescmdlst(t_scmd_list *lst)
{
	t_scmd_list	*temp;
	t_scmd_list	*next;

	if (!lst)
		return ;
	temp = lst;
	while (temp)
	{
		next = temp->next;
		free(temp);
		temp = next;
	}
	lst = NULL;
}

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

void	ft_freelst(t_list *lst)
{
	t_list	*temp;
	t_list	*next;

	if (!lst)
		return ;
	temp = lst;
	while (temp)
	{
		next = temp->next;
		free(temp);
		temp = next;
	}
	lst = NULL;
}
