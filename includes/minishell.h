/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 11:04:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/15 14:13:49 by rvan-aud         ###   ########.fr       */
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
# include "libft.h"

# define MAX_PATH_LEN 4096

typedef struct	s_redirection
{
	char	*name;
	int		flag;
	int		fd;
}				t_redirection;

typedef struct	s_command
{
	char	*cmd;
	int		argc;
	char	**argv;
}				t_command;

typedef struct	s_script
{
	t_command		*commands;
	int				cmd_count;
	t_redirection	out;
	t_redirection	in;
	char			**envp;
}				t_script;

int builtin_pwd();
int	builtin_env(char **envp);

#endif