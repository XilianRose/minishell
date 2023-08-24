/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_utilities.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:25:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/24 16:44:34 by cschabra      ########   odam.nl         */
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
