/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 14:10:44 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/18 15:29:39 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array != NULL && array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	if (array != NULL)
	{
		free(array);
		array = NULL;
	}
}

void	print_array(char **array)
{
	while (array[0] != '\0')
	{
		printf("array str: [%s]\n", *array);
		array++;
	}
}

void	print_list(t_list *list)
{
	while (list != NULL && list->next != NULL)
	{
		printf("list: [%s]\n", list->content);
		list = list->next;
	}
	if (list != NULL)
		printf("list: [%s]\n", list->content);
}

size_t	make_tlist(char **ui_array, t_list **tokens)
{
	t_list	*token;
	char	*str;

	while (ui_array[0] != '\0')
	{
		str = *ui_array;
		token = ft_lstnew(str);
		if (token == NULL)
			return (ft_lstclear(tokens, &free), 1);
		ft_lstadd_back(tokens, token);
		ui_array++;
	}
	return (0);
}

t_list	*tokenisation(char *user_input)
{
	t_list	*tokens;
	char	**ui_array;

	tokens = NULL;
	ui_array = ft_split(user_input, ' ');
	if (!ui_array)
		exit(1);
	print_array(ui_array);
	make_tlist(ui_array, &tokens);
	print_list(tokens);
	return (free_array(ui_array), tokens);
}

int	main(void)
{
	char	*user_input;

	user_input = "cat << \";\" | grep \"aap noot mies\" | wc -l > outfile";
	tokenisation(user_input);
	return (0);
}
