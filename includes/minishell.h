/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 11:04:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/27 14:57:12 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/stat.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <limits.h>
# include <errno.h>
# include "libft.h"
# include <termios.h>
# include <curses.h>
# include <term.h>

# define MAX_PATH_LEN 4096

int g_pid;

typedef enum e_token_type
{
	TOKEN_EAT,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_NAME
}			t_token_type;

typedef struct s_operations
{
	const char		*op;
	int				size;
	t_token_type	type;
}				t_operations;

typedef struct s_token
{
	char			*content;
	int				size;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct s_redirection
{
	char	*name;
	int		flag;
}				t_redirection;

typedef struct s_command
{
	char			*cmd;
	int				argc;
	char			**argv;
	t_redirection	out;
	t_redirection	in;
}				t_command;

typedef struct s_script
{
	t_command		*commands;
	int				cmd_count;
	int				exit_status;
	char			**envp;
	struct termios	termios_p;
}				t_script;

int				parse(t_script *script, char **line_buf);
int				tokenizer(char *str, t_token **head);
void			replace_env_var(t_token *head, t_script *script);

// Tokenizer utils
t_token			*create_token(const char *string, int size, t_token_type type);
void			add_token(t_token **head, t_token *new_token);
t_operations	search_token_type(const char *s);

// Exec
void			exec_cmd( char **path, char **cmd, char **env);
int				handle_cmd(t_script *script);
int				check_builtin(char *cmd);
int				handle_builtin(int ret, t_script *script, int i);
int				pipex(t_script *script, char **path_env);
char			**split_paths(char **env);
void			first_child(t_script *script, char **path_env, int *pipe1);
void			middle_child(t_script *script, char **path_env, int *pipein, int *pipeout, int i);
void			last_child(t_script *script, char **path_env, int *pipein, int i);
void			single_cmd(t_script *script, char **path_env);

// Signals
void			sig_handler(int signum);

// Builtins
int				builtin_echo(t_command command);
int				builtin_cd(t_command command);
int 			builtin_exit(t_command command, t_script *script);
int				builtin_pwd(void);
int				builtin_export(t_script *script, t_command command);
int				builtin_unset(t_script *script, t_command command);
int				builtin_env(char **envp);

// Utils
char			*ft_trim_quotes(char *str);
int				get_cmd_count(char *line_buf);
int				return_error(const char *msg);
void			get_num_args(t_token *head, t_script *script);
void			in_redir(t_script *script, int i, char **path_env);
void			out_redir(t_script *script, int i, char **path_env);
void			close_pipes(int *pipe1, int *pipe2);
int				pipe_dup(int *pipe, int mod, int std);
void			cmd_builtin(t_script *script, char **path_env, int ret, int i);
void			heredoc(t_script *script, int i, char **path_env);
int				ft_putchar(int c);

// Free
void			free_tokens(t_token *head);
void			free_commands(t_script *script);
void			free_path_env(char **path_env);
void			free_cmds_path(t_script *script, char **path_env);
void			close_free_exit(t_script *s, char **path_env, int *p1, int *p2);

#endif