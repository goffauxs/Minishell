/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 11:04:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/21 16:30:23 by sgoffaux         ###   ########.fr       */
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
	int		fd;
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
	t_command	*commands;
	int			cmd_count;
	char		**envp;
}				t_script;

int				parse(t_script *script);
int				tokenizer(char *str, t_token **head);
void			replace_env_var(t_token *head, char **envp);

// Tokenizer utils
t_token			*create_token(const char *string, int size, t_token_type type);
void			add_token(t_token **head, t_token *new_token);
t_operations	search_token_type(const char *s);
int				treat_quotes(char **str);

// Free
void			free_tokens(t_token *head);
void			free_commands(t_script *script);

// Utils
char			*ft_trim_quotes(char *str);
int				get_cmd_count(char *line_buf);
int				return_error(const char *msg);
void			get_num_args(t_token *head, t_script *script);

#endif