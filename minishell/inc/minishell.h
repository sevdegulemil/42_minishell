/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seemil <seemil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 12:24:36 by seemil            #+#    #+#             */
/*   Updated: 2024/10/01 12:43:13 by seemil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h> 
# include <stdbool.h>
# include <errno.h>
# include <string.h>
# include <dirent.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include "sys/wait.h"
# include "signal.h"
# include "sys/types.h"

# define PATH_MAX 4096

# define SUCCESS 0
# define FAILURE -1
# define HANDLED -2

# define NAFD -2

# define IN_HEREDOC 2
# define AFTER_HEREDOC 3
# define IN_CMD 4
# define AFTER_CMD 5

# define ENO_OTHER 40000

# define ERR_NO_SUCH_FILE_OR_DIR 30002
# define ERR_NUMERIC_ARG_REQUIRED 30003
# define ERR_TOO_MANY_ARG 30004
# define ERR_NOT_A_VALID_IDENTIFIER 30005
# define ERR_PERMISSION_DENIED 30006
# define ERR_PERMISSION_DENIED_BROKEN_PIPE 30007
# define ERR_IS_DIR 30008

# define ERR_HOME_NOT_SET 10001
# define ERR_CMD_NOT_FOUND 10002
# define ERR_CANT_CHANGE_DIR 10003
# define ERRP_NOT_A_VALID_IDENTIFIER 10004
# define ERR_NOT_VALID_IN_THIS_CTX 10005
# define ERR_OLDPWD_NOT_SET 10006
# define ERR_FORK_ERROR 10007

# define ENDL "\n"
# define ESTR_UNKNOWN "unknown error"
# define ESTR_UNEXPECTED "unexpected error"
# define ESTR_CMD_NOT_FOUND "command not found"
# define ESTR_HOME_NOT_SET "HOME not set"
# define ESTR_CANT_CHANGE_DIR "cannot change directory"
# define ESTR_INVALID_ARG "invalid argument"
# define ESTR_NOT_A_VALID_IDENTIFIER "not a valid identifier"
# define ESTR_NO_SUCH_FILE_OR_DIR "No such file or directory"
# define ESTR_NUMERIC_ARG_REQUIRED "numeric argument required"
# define ESTR_TOO_MANY_ARG "too many arguments"
# define ESTR_PERMISSION_DENIED "permission denied"
# define ESTR_PERMISSION_DENIED_BROKEN_PIPE "permission denied"
# define ESTR_IS_DIR "is a directory"
# define ESTR_NOT_VALID_IN_THIS_CTX "not valid in this context"
# define ESTR_SYN_UNKNOWN_ERR "unknown syntax error"
# define ESTR_SYN_ZERO_PIPE "syntax error near unexpected token `newline'"
# define ESTR_SYN_EMPTY_AFTER "syntax error or near unexpected token `newline'"
# define ESTR_SYN_MISS_QUOTE "unexpected quote `'', `\"'"

# define COLOR "\e[0;34m"
# define COLOR_RED "\e[0;31m" 
# define COLOR_RESET "\e[0m"

# define PROMPT "minishell$ "

extern int			g_sig;

typedef struct s_syntax
{
	unsigned char	duplex;
	unsigned char	simplex;
	unsigned char	zero_pipe;
	unsigned char	undefined;
}					t_syntax;

typedef enum e_token_type
{
	NONE,
	CMD,
	ARG,
	PIPE,
	RED_INPUT,
	RED_HEREDOC,
	RED_OUTPUT,
	RED_APPEND,
	RED_FILE,
	RED_HEREDOC_TYPE
}					t_token_type;

typedef struct s_token
{
	char			*data;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_token_append_meta_data
{
	t_token_type	type;
	int				in_quote;
	bool			has_meta;
	int				i;
	int				start;
}					t_token_append_meta_data;

typedef struct s_token_sep_md
{
	t_token			**token_arr;
	t_token			*iter;
	t_token			*temp;
	t_token			*temp_root;
	int				i;
}					t_token_sep_md;

typedef struct s_state
{
	int				status;
	char			**argv;
	char			**env;
	char			*prompt;
	t_token			**token_arr;
	int				cmd_ct;
	int				err;
}					t_state;

typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	int				in;
	int				out;
	int				*heredoc;
	int				idx;
	int				count;
	int				bin;
	int				bout;
}					t_cmd;

//	lexer	//
void				assign_token_arr_types(t_token **token_arr);
void				assign_token_types(t_token *token);
void				handle_dollar(t_token **root, t_state *state);
int					handle_special_dollar(char **data, int start, int i, \
					t_state *state);
void				handle_number_dollar(char **data, int start, int i);
int					handle_regular_dollar(char **data, int start, int i, \
					t_state *state);
char				*get_dollar_value(char *key, t_state *state);
char				*create_data_from_dollar(char *data, char *value, \
					int start, int index);
void				extract_dollar_key_values(char **data, t_state *state, \
					bool *has_dollar);
bool				str_is_all_space(const char *str);
bool				token_is_all_space(t_token *root);
t_token				**run_lexer(t_state *state);
t_token				*extract_meta_chars(t_token **root);
bool				is_meta(t_token_type type);
bool				is_meta_char(char *data, int i);
t_token_type		get_meta_type(char *data, int i);
int					pass_quoted_str(char *p, int *oi);
char				get_in_quote(char old, char data);
bool				is_unnecessary_quote(int *quote, char data);
int					count_unnecessary_quotes(char *data);
bool				has_unnecessary_quotes(char *data);
t_token				*separate_prompt_by_space(char *prompt);
int					create_separated_node(t_token **root, char *prompt, \
					int start, int i);
int					pass_data(char *prompt, int *i);
void				token_insert_dollar_nodes(t_token **token);
void				token_insertion(t_token **token, t_token *temp, \
					t_token *sub_nodes);
void				ctrl_d(int sig);
void				ctrl_c(int sig);
void				tcseta(void);
void				handle_signals(void);
int					is_space(char character);
void				syntax_other(t_state *shell, t_syntax *syntax, int *i);
int					choose(t_state *shell, t_syntax *syntax, int *i);
int					syntax_check(t_state *shell);
void				print_syntax_err(int errs, t_state *state);
t_token				*token_new(char *data, t_token_type type);
t_token				*token_add_last(t_token *token, t_token *new);
void				token_add_prev(t_token **token, t_token *new);
bool				token_append_meta(t_token **token);
void				token_append_all(t_token **token, int start, int i, \
					t_token_type type);
void				token_append_meta_data_init(t_token_append_meta_data *md, \
					t_token **token);
int					token_append_str(t_token **token, int start, int i);
char				**token_to_arg(t_token *token, char *cmd_path);
void				token_dispose(t_token **token);
void				token_dispose_all(t_token **token);
void				token_arr_dispose(t_token ***token_arr);
bool				token_sep_md_init(t_token_sep_md *md, t_token *token);
t_token				**token_separate_by_pipe(t_token *token);

//	executor	//
void				exec_start(t_state *state);
int					run_executor(t_state *state);
int					exec_single_command(t_token *token, t_state *state, \
					t_cmd *command);
int					exec_single_command_prepare(t_token *token, \
					t_state *state, t_cmd *cmd);
int					which_command_built_in(t_state *state, t_token *token, \
					t_cmd *cmd, int **pipe_fds);
void				built_in_handle_fds(t_cmd *cmd, int **pipe_fds);
int					run_cd(t_state *state, t_token *token);
int					run_pwd(t_cmd *cmd, t_state *state);
int					run_export(t_state *state, t_token *token, t_cmd *cmd);
int					run_env(t_state *state, t_cmd *cmd);
int					run_echo(t_state *state, t_token *token, t_cmd *cmd);
int					run_unset(t_state *state, t_token *token);
int					run_exit(t_state *state, t_token *token);
char				*get_env_value(t_state *state, const char *key);
int					env_set_value(t_state *state, char *key_value);
int					env_set_pwd(t_state *state);
void				cmd_dispose(t_cmd *cmd);
char				*get_env_path_arr_as_str(char **env);
char				*get_cmd_absolute_path(t_token *token, t_state *state);
char				*get_cmd_path(t_token *token, t_state *state);
int					handle_red_input(t_token *token, t_cmd *cmd, \
					bool has_last_heredoc, t_state *state);
int					handle_red_heredoc(t_token *token, t_cmd *cmd, int i);
int					handle_red_output(t_token *token, t_cmd *cmd, \
					t_state *state);
int					handle_red_append(t_token *token, t_cmd *cmd, \
					t_state *state);
int					set_other_redirect(t_token *token, t_cmd *cmd, \
					t_state *state);
int					handle_fds(t_token *token, t_cmd *cmd, t_state *state, \
					bool has_last_heredoc);
int					set_heredoc(t_token *token, t_cmd *cmd, int i);
int					**pipe_fd_init(int pipe_count);
int					**pipe_fd_dispose_idx(int **pipe_fd, int i);
int					fork_init_exec_child_part(t_state *state, t_cmd *cmd, \
					pid_t *pids, int **fd);
int					fork_init(t_state *state, t_cmd *cmd, int **fd, \
					int arr_len);
void				handle_child_process(int **fd, t_state *state, \
					t_cmd *cmd, int i);

//	utils	//
int					ft_strlen(const char *s);
int					ft_strncmp(const char *s1, const char *s2, int n);
int					ft_strcmp(char *s1, char *s2);
char				*ft_strdup(const char *src);
char				*ft_substr(char const *s, int start, int len);
char				*ft_itoa(int n);
char				*ft_strjoin(char const *s1, char const *s2, int flag_free);
int					ft_atoi(const char *str);
char				*ft_strchr(const char *s, int c);
bool				is_al_underscore(char c);
char				**str_arr_append(char **str_arr, char *data);
t_token				*token_get_root(t_token *node);
t_token				*token_get_last(t_token *node);
bool				token_is_just_meta(t_token **token);
void				token_old_del(t_token **temp, t_token *root);
int					token_count_pipe(t_token *token);
int					token_arr_len(t_token **token_arr);
int					token_count_args(t_token *token);
int					set_cmd_arg_and_path(t_token *token, t_state *state, \
					t_cmd *cmd, int **pipe_fds);
int					token_append_meta_pipe(t_token **token);
int					token_append_meta_redl(t_token **token);
int					token_append_meta_redll(t_token **token);
int					token_append_meta_redr(t_token **token);
int					token_append_meta_redrr(t_token **token);
void				syntax_squote(t_syntax *syntax);
void				syntax_dquote(t_syntax *syntax);
int					syntax_pipe(t_state *shell, t_syntax *syntax, int *i);
int					syntax_sarrow(t_syntax *syntax, int *i);
int					syntax_darrow(t_syntax *syntax, int *i);
bool				token_is_built_in(t_token *token);
void				state_dispose(t_state **state);
void				dispose_prompt(t_state *state);
void				state_dispose_single(t_state **state);
void				dispose_paths(char **paths);
bool				is_valid_dollar(char *data, int i);
bool				is_digit(char c);
bool				is_alpha(char c);
bool				is_alnum_underscore(char c);
void				print_unknown_err(t_state *state);
int					print_execute_err(t_state *state, const t_token *token, \
					int status, int err);
void				print_fatal_err(const char *msg, const int err);
char				**ft_split(char const *str, char c);
void				dprint(int fd, const char *s);
void				dprintln(int fd, const char *s);
void				eprint(const char *str);
void				eprintln(const char *str);
void				delete_unnecessary_quotes(char **data);
void				handle_unnecessary_quotes(t_token *root);
bool				token_has_cmd(t_token *token);
int					cmd_init(t_cmd *cmd, int arr_len);
bool				is_empty_arg(t_token *arg);
int					env_len(t_state *state);
char				**str_arr_remove(char **str_arr, char *key);
int					w_exit_status(int status);

#endif