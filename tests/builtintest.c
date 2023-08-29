/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtintest.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/04 14:48:42 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/29 13:28:01 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

// to test exit i have to return the exit function..
void	ft_test_exit(t_env *env)
{
	t_cmd	*info;
	char	**arg;
	int		j;

	j = 0;
	arg = ft_split("exit 9223372036854775806", ' ');
	info = NULL;
	info = allocate_mem_cmd_info(arg, env, true);
	ft_exit_builtin(info);
	while (arg[j])
		free(arg[j++]);
	free(arg);
	free(info);
}

void	ft_test_echo(t_env *env)
{
	t_cmd	*info;
	char	**arg;
	int		i;

	i = 0;
	arg = ft_split("echo -n -n blep -n -n -n", ' ');
	info = NULL;
	info = allocate_mem_cmd_info(arg, env, true);
	ft_echo_builtin(info);
	while (arg[i])
		free(arg[i++]);
	free(arg);
	free(info);
}

void	ft_test_cd(t_env *env)
{
	t_cmd	*info;
	char	**arg;
	int		i;

	i = 0;
	arg = ft_split("cd ../Minishell/srcs", ' ');
	info = NULL;
	info = allocate_mem_cmd_info(arg, env, true);
	ft_cd_builtin(info);
	while (arg[i])
		free(arg[i++]);
	free(arg);
	free(info);
}

void	ft_test_pwd(void)
{
	ft_pwd_builtin();
}

void	ft_test_env(t_env *env)
{
	t_cmd	*info;
	char	**arg;
	int		i;

	i = 0;
	arg = ft_split("env", ' ');
	info = NULL;
	info = allocate_mem_cmd_info(arg, env, true);
	ft_env_builtin(info);
	while (arg[i])
		free(arg[i++]);
	free(arg);
	free(info);
}

void	ft_test_export(t_env *env)
{
	t_cmd	*info;
	char	**arg;
	t_cmd	*info2;
	char	**arg2;
	int		i;

	i = 0;
	info = NULL;
	arg = ft_split("export _+= _= _ __=ok _s0_=kek _s0=ok s0_=ok TERM USER+= here=replaceme here=ok this=a b c noexist+=ok b=ok test=ap/pen+dhere test+=sumfin = 2test=sumfin", ' ');
	info = allocate_mem_cmd_info(arg, env, true);
	ft_export_builtin(info, env);
	while (arg[i])
		free(arg[i++]);
	free(arg);
	arg2 = ft_split("export", ' ');
	info2 = NULL;
	info2 = allocate_mem_cmd_info(arg2, env, true);
	ft_export_builtin(info2, env);
	i = 0;
	while (arg2[i])
		free(arg2[i++]);
	free(arg2);
	free(info2);
	arg = ft_split("env", ' ');
	info->arg = arg;
	ft_env_builtin(info);
	i = 0;
	while (arg[i])
		free(arg[i++]);
	free(arg);
	free(info);
}

void	ft_test_unset(t_env *env)
{
	t_cmd	*info;
	char	**arg;
	t_cmd	*info2;
	char	**arg2;
	int		i;

	i = 0;
	info = NULL;
	arg = ft_split("unset _ __ _0test PATH TERM 1test HOME thismust.error=something test=error test+=error", ' ');
	info = allocate_mem_cmd_info(arg, env, true);
	ft_unset_builtin(info);
	while (arg[i])
		free(arg[i++]);
	free(arg);
	arg2 = ft_split("env", ' ');
	info2 = NULL;
	info2 = allocate_mem_cmd_info(arg2, env, true);
	ft_env_builtin(info2);
	i = 0;
	while (arg2[i])
		free(arg2[i++]);
	free(arg2);
	free(info);
	free(info2);
}

// static void	ft_leaks(void)
// {
// 	system("leaks -q minishell_test");
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	// char	buffer[MAXPATHLEN];
// 	t_env	env;
// 	int		i;

// 	atexit(ft_leaks);
// 	argc = 0;
// 	argv = 0;
// 	i = 0;
// 	ft_copy_env(&env, envp);
// 	// while (env.new_env[i])
// 	// 	printf("%s\n", env.new_env[i++]);
// 	// puts("-----------------------------------------------------");
// 	// ft_test_env(&env);
// 	// ft_test_export(&env);
// 	// ft_test_unset(&env);
// 	// ft_test_exit(&env);
// 	// ft_test_echo(&env);

// 	// if (getcwd(buffer, MAXPATHLEN) == NULL)
// 	// 	ft_throw_error(errno, "pwd failed");
// 	// ft_putendl_fd(buffer, STDOUT_FILENO);
// 	// ft_test_cd(&env);
// 	// if (getcwd(buffer, MAXPATHLEN) == NULL)
// 	// 	ft_throw_error(errno, "pwd failed");
// 	// ft_putendl_fd(buffer, STDOUT_FILENO);

// 	// ft_test_pwd();
// 	ft_free_env(env.new_env, NULL);
// 	// while (1);
// }
