/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 11:04:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/15 15:10:00 by mdeclerf         ###   ########.fr       */
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
# include "libft.h"
# include <limits.h>
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
	t_redirection	out;
	t_redirection	in;
}				t_command;

typedef struct	s_script
{
	t_command		*commands;
	int				cmd_count;
	char			**envp;
}				t_script;

int		builtin_echo(t_command *test);
int		builtin_cd(t_command *test);
void	builtin_exit();
int		builtin_pwd();
int		builtin_env(char **envp);
int		builtin_export(t_script *script);

void	sig_handler(int signum);
void	rl_replace_line (const char *text, int clear_undo);

#endif