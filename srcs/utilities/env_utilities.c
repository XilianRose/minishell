/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utilities.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:25:04 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/10 13:31:36 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	ft_free_env(env->new_env, NULL);
	free(env);
}
