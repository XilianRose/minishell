/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: cschabra <cschabra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/01 14:22:00 by cschabra      #+#    #+#                 */
/*   Updated: 2023/08/10 16:53:04 by cschabra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/errno.h>
# include <sys/param.h>
# include <signal.h>
# include <stdbool.h>

typedef enum e_struct_type
{
	CMD = 0,
	RDR = 1
}	t_struct_type;

typedef enum e_rdr_type
{
	RDR_INPUT,
	RDR_OUTPUT,
	HERE_DOC,
	RDR_APPEND
}	t_rdr_type;

typedef enum e_token_type
{
	CMD_OR_FILE_TOKEN	= 1,
	RDR_TOKEN	= 2,
	PIPE_TOKEN			= 3
}	t_token_type;

typedef struct s_rdr
{
	char		*data;
	t_rdr_type	type;
}	t_rdr;

typedef struct s_env
{
	char	**new_env;
	int		env_len;
}	t_env;

typedef struct s_cmd
{
	char	*path;
	char	**arg;
	t_env	*env;
	bool	builtin;
}	t_cmd;

typedef struct s_scmd_list
{
	void				*data;
	t_struct_type		type;
	struct s_scmd_list	*next;
}	t_scmd_list;

typedef struct s_export
{
	char	**new_env;
	char	*arg_copy;
	int		arg_len;
	int		var_len;
	int		name_len;
	char	*new_var;
}	t_export;

typedef struct s_childproc
{
	pid_t		*ids;
	t_cmd		*cmd;
	int			status;
	int			errorcode;
	int			i;
	int			nr_of_cmds;
	int			pipe_count;
	int			**pipes;
	int			fdin;
	int			fdout;
	int			oldout;
	t_scmd_list	*temp;
}	t_childproc;

typedef struct s_token
{
	char			*data;
	t_token_type	type;
}	t_token;

typedef struct s_expand_info
{
	int		i;
	int		j;
	int		dquote_i;
	int		quote_i;
	char	*old_data;
	char	*expanded_data;
}	t_expand_info;

typedef struct s_expand_length_info
{
	int		i;
	int		length;
	int		dquote_i;
	int		quote_i;
	char	*data;
}	t_expand_length_info;

void			ft_test_child(t_env *env, char **argv); // tester, remove at end
void			ft_test_signals(void); // tester, remove at end
// builtins
void			ft_echo_builtin(t_cmd *info);
void			ft_cd_builtin(t_cmd *info);
void			ft_pwd_builtin(void);
void			ft_exit_builtin(t_cmd *info);

void			ft_free_env(char **new_env, char *arg_copy);
void			ft_env_builtin(t_cmd *info);
void			ft_copy_env(t_env *env, char **old_env);

void			ft_export_append(t_cmd *info, t_env *env, char *arg, int j);

void			ft_add_new_var(t_cmd *info, t_env *env, char *arg);
void			ft_export_builtin(t_cmd *info, t_env *env);

void			ft_fill_env(t_cmd *info, t_env *env, t_export *exp, int i);
void			ft_check_for_plus(char *arg);
int				ft_find_value(char *var);
void			ft_write_export(char **sortedenv);

void			ft_unset_builtin(t_cmd *info);

// execution
void			ft_create_child(t_list *lst);

bool			ft_prep(t_childproc *child, t_list *lst);

void			ft_free_all(void);
void			ft_error_export_unset(char *name, char *option);
void			ft_error_exit(char *str);
void			ft_error_env(int errnr, char *str);
void			ft_throw_error(int errnr, char *str);

void			ft_execute(t_cmd *info);

void			ft_restore_output(t_childproc *child);
void			ft_close_fds(t_childproc *child);
bool			ft_infile(t_childproc *child, t_rdr *which);
bool			ft_outfile(t_childproc *child, t_rdr *which);
void			ft_check_for_files(t_childproc *child, t_scmd_list *lst);

void			ft_heredoc(char *data);

void			ft_free_pipes(int **pipes, int pipe_count);
int				ft_count_pipes(t_list *arglst);
int				**ft_create_pipes(int **pipes, int pipe_count);

// expander here
// -----------------------------------------------------------------------------

// lexer
t_list			*quote_begin(t_list *tokens);
t_list			*quote_end(t_list *tokens);
t_token			*new_token(const char *data, t_token_type type);
t_list			*tokenisation(const char *user_input);

// parser here
// -----------------------------------------------------------------------------

t_list			*parse(const char *user_input);
size_t			count_cmdtokens(t_list **tokens);
bool			is_builtin(t_list **tokens);

// utilities
long long int	ft_atollong(const char *str);

void			ft_bubble_sort(char **sortedenv, int len);

t_scmd_list		*ft_lstnewscmd(void *data, t_struct_type type);
void			ft_freescmdlst(t_scmd_list *lst);
void			scmdlst_add_back(t_scmd_list **scmds, t_scmd_list *new);
void			ft_freelst(t_list *lst);

t_rdr			*allocate_mem_rdr(char *data, t_rdr_type type);
t_cmd			*allocate_mem_cmd_info(char **arg, t_env *env, bool builtin);

bool			str_contains_any(const char *str, const char *chars);
bool			str_equals(char *lhs, char *rhs);
bool			ft_cmpname(const char *s1, const char *s2, int n);
void			free_array(char **array);

#endif
