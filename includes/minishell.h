/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 11:04:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/21 14:48:08 by sgoffaux         ###   ########.fr       */
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

int		parse(t_script *script);
void	free_commands(t_script *script);

#endif