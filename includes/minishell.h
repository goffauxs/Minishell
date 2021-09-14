/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 11:04:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/14 17:54:51 by mdeclerf         ###   ########.fr       */
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
# define MAX_PATH_LEN 4096

typedef struct	s_command
{
	char	*cmd;
	int		argc;
	char	**argv;
}				t_command;

typedef struct	s_script
{
	t_command	*commands;
	char		*outfile;
	char		*infile;
	char		*err;
}				t_script;

int		builtin_echo(t_command *test);
int		builtin_cd(t_command *test);
void	builtin_exit();
int		builtin_pwd();
int		builtin_env(char **envp);

void	sig_handler(int signum);

#endif