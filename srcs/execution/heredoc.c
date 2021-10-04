/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:51 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 14:45:05 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_message_heredoc(char *content)
{
	ft_putstr_fd("Minishell: warning: here-document delimited", 2);
	ft_putstr_fd("by end-of-file (wanted '", 2);
	ft_putstr_fd(content, 2);
	ft_putendl_fd("\')", 2);
}

static void	loop_heredoc(t_script *script, int pipe, int i)
{
	char	*tmp;
	char	*bis;
	t_list	*heredoc_tmp;

	bis = "";
	heredoc_tmp = script->commands[i].in.heredoc;
	while (g_exit_status != 1 && heredoc_tmp)
	{
		tmp = readline("\033[0;32m> \033[0m");
		if (!tmp || !ft_strncmp(tmp, heredoc_tmp->content,
				ft_strlen(heredoc_tmp->content) + 1))
		{
			if (!tmp)
				error_message_heredoc(heredoc_tmp->content);
			heredoc_tmp = heredoc_tmp->next;
			continue ;
		}
		if (!heredoc_tmp->next)
		{
			tmp = ft_strjoin(tmp, "\n");
			bis = ft_strjoin(bis, tmp);
		}
	}	
	write(pipe, bis, ft_strlen(bis));
}

void	heredoc(t_script *script, int i, char **path_env)
{
	int		pipe_tmp[2];

	if (pipe(pipe_tmp) == -1)
	{
		write(2, "Error: pipe failed\n", 19);
		free_cmds_path(script, path_env);
		exit(1);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handler_heredoc);
	loop_heredoc(script, pipe_tmp[1], i);
	if (pipe_tmp[0] != STDIN_FILENO)
	{
		if (dup2(pipe_tmp[0], STDIN_FILENO) == -1)
		{
			write(2, "Error: dup2 failed\n", 19);
			close_pipes(pipe_tmp, NULL);
			free_cmds_path(script, path_env);
			exit(1);
		}
	}
	close(pipe_tmp[0]);
	close(pipe_tmp[1]);
}
