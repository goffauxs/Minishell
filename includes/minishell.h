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
# include "libft.h"

# define MAX_PATH_LEN 4096

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

static const t_operations	ops[] = 
{
	{">>", 2, TOKEN_REDIR_OUT},
	{"<<", 2, TOKEN_REDIR_IN},
	{"|", 1, TOKEN_PIPE},
    {">", 1, TOKEN_REDIR_OUT},
    {"<", 1, TOKEN_REDIR_IN},
    {" ", 1, TOKEN_EAT},
    {"\n", 1, TOKEN_EAT},
    {"\v", 1, TOKEN_EAT},
    {"\t", 1, TOKEN_EAT},
    {"\r", 1, TOKEN_EAT},
    {"\f", 1, TOKEN_EAT},
    {NULL, 1, 0}
};

typedef struct s_token
{
	char			*content;
	int				size;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct	s_redirection
{
	char	*name;
	int		flag;
	int		fd;
}				t_redirection;

typedef struct	s_command
{
	int				argc;
	char			**argv;
	t_redirection	out;
	t_redirection	in;
}				t_command;

typedef struct	s_script
{
	t_command	*commands;
	int			last_exit_status;
	int			cmd_count;
	char		**envp;
}				t_script;

int exit_status;
//int g_exit_status;

/*
** main.c
*/
char		*get_prompt();

/*
** parsing.c
*/
// int			get_cmd_count(char *line_buf);
// int			set_redir_flag(char c, char chevron);
// int			get_inoutfile(char *line_buf, char chevron, t_redirection *redir, int start);
// char		*trim_infile(char *str);
// char		*trim_outfile(char *str);
// t_command	parse_command(char *split_buf);
int				parse(t_script *script, char **line_buf);
int				tokenizer(char *str, t_token **head);
void			replace_env_var(t_token *head, char **envp);

// Tokenizer utils
t_token			*create_token(const char *string, int size, t_token_type type);
void			add_token(t_token **head, t_token *new_token);
t_operations	search_token_type(const char *s);

// Free
void			free_tokens(t_token *head);
void			free_commands(t_script *script);

// Utils
char			*ft_trim_quotes(char *str);
int				get_cmd_count(char *line_buf);
int				return_error(const char *msg);
void			get_num_args(t_token *head, t_script *script);


/*
** exec.c
*/
//static char	*add_forw_slash(char *str);
//static int	check_path_line(char **env);
//static void	init_vars(int *i, int *j);
void	exec_cmd( char **path, char **cmd, char **env);
// void	child(char **path_env, t_script script, int i);
void	handle_cmd(t_script *script);
int		check_builtin(char *cmd);
int		handle_builtin(int ret, t_script *script, int i);

/*
** exec_pipes.c
*/
int		pipex(t_script *script, char **path_env);

/*
** signal.c
*/
void		sig_handler(int signum);

/*
** builtins.c
*/
int			builtin_echo(t_command command);
int			builtin_cd(t_command command);
int			builtin_exit(t_command command);
int			builtin_pwd(void);
int			builtin_export(char ***envp, t_command command);
int			builtin_env(char **envp);

/*
** path_handling.c
*/
char	**split_paths(char **env);

#endif