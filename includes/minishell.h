/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 11:04:53 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/17 16:23:26 by mdeclerf         ###   ########.fr       */
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

typedef struct	s_globale
{
	int	exit_status;
	int	running_pid;
}	s_globale;

typedef struct	s_redirection
{
	char	*name;
	int		flag;
	int		fd;
}				t_redirection;

typedef struct	s_command
{
	char			*cmd;
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

s_globale glo;

/*
** main.c
*/
char		*get_prompt();

/*
** parsing.c
*/
int			get_cmd_count(char *line_buf);
int			set_redir_flag(char c, char chevron);
int			get_inoutfile(char *line_buf, char chevron, t_redirection *redir, int start);
char		*trim_infile(char *str);
char		*trim_outfile(char *str);
t_command	parse_command(char *split_buf);

/*
** exec.c
*/
//static char	*add_forw_slash(char *str);
//static int	check_path_line(char **env);
//static void	init_vars(int *i, int *j);
//static char	**split_paths(char **env);
//static void	exec_cmd( char **path, char **cmd, char **env);
void	child(char **path_env, t_script script, int i);
void	handle_cmd(t_script script, int i);
int		check_builtin(char *cmd);
int		handle_builtin(int ret, t_script script, int i);

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
int			builtin_export(t_script *script);
int			builtin_env(char **envp);

#endif