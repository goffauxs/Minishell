/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 11:04:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/16 15:32:51 by sgoffaux         ###   ########.fr       */
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

typedef enum e_token_state
{
	DEFAULT,
	QUOTES,
	DQUOTES
}			t_token_state;

typedef enum e_pointer_state
{
	START,
	IN_WORD,
	IN_STRING
}			t_pointer_state;

typedef struct s_token
{
	char			*string;
	t_token_state	state;
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

#endif