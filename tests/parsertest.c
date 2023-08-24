/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsertest.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mstegema <mstegema@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/24 16:48:04 by mstegema      #+#    #+#                 */
/*   Updated: 2023/08/24 16:48:34 by mstegema      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*user_input;
	t_list	*tokens;

	tokens = NULL;
	// user_input = "cat << \";\" | grep \"aap noot mies\" | wc -l > outfile | >l";
	user_input = "cat << \";\" | grep \"aap | wc -l > outfile | >l";
	printf("user input: %s\n______________________________________________________________________\n", user_input);
	tokens = tokenisation(user_input);
	printf("______________________________________________________________________\n");
	print_tlist(tokens);
	return (0);
}
