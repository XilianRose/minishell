/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_utilities.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:25:13 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/10 13:31:43 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_freelst(t_list *lst)
{
	t_list	*temp;
	t_list	*next;

	if (!lst)
		return (false);
	temp = lst;
	while (temp)
	{
		next = temp->next;
		free(temp);
		temp = next;
	}
	lst = NULL;
	return (false);
}

bool	ft_freescmdlst(t_scmd_list *lst)
{
	t_scmd_list	*temp;
	t_scmd_list	*next;

	if (!lst)
		return (false);
	temp = lst;
	while (temp)
	{
		next = temp->next;
		free(temp);
		temp = next;
	}
	lst = NULL;
	return (false);
}

void	lst_free(t_list **lst)
{
	t_list	*current;
	t_list	*next;

	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*lst = NULL;
}
