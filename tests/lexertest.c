/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexertest.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/24 15:09:45 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/28 14:18:36 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	print_array(char **array)
// {
// 	while (array[0] != '\0')
// 	{
// 		printf("array str: [%s]\n", *array);
// 		array++;
// 	}
// }

// static void	print_tlist(t_list *list)
// {
// 	t_token	*current;

// 	current = list->content;
// 	while (list != NULL && list->next != NULL)
// 	{
// 		printf("data: [%s]	type: [%d]\n", current->data, current->type);
// 		list = list->next;
// 		current = list->content;
// 	}
// 	if (list != NULL)
// 		printf("data: [%s]	type: [%d]\n", current->data, current->type);
// }

// t_list	*tokenisation(const char *user_input)
// {
// 	t_list	*tokens;
// 	char	**ui_array;

// 	tokens = NULL;
// 	ui_array = ft_split(user_input, ' ');
// 	if (!ui_array)
// 		exit(1); //exit "failed to parse"?
// 	print_array(ui_array);
// 	make_tlist((const char **) ui_array, &tokens);
// 	//error handling -> exit "failed to parse"?
// 	printf("\nbefore merge	(CMD_OR_FILE = 1, REDIRECTION = 2, PIPE = 3)\n");
// 	printf("______________________________________________________________________\n");
// 	print_tlist(tokens);
// 	merge_tokens(tokens);
// 	printf("\nafter merge	(CMD_OR_FILE = 1, REDIRECTION = 2, PIPE = 3)\n");
// 	printf("______________________________________________________________________\n");
// 	print_tlist(tokens);
// 	return (free_array(ui_array), tokens);
// }

// int	main(void)
// {
// 	char	*user_input;
// 	t_list	*tokens;

// 	tokens = NULL;
// 	// user_input = "cat << \";\" | grep \"aap noot mies\" | wc -l > outfile | >l";
// 	user_input = "cat << \";\" | grep \"aap | wc -l > outfile | >l";
// 	printf("user input: %s\n______________________________________________________________________\n", user_input);
// 	tokens = tokenisation(user_input);
// 	printf("______________________________________________________________________\n");
// 	print_tlist(tokens);
// 	return (0);
// }










