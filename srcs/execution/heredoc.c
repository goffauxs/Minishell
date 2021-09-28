/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:51 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/27 18:12:05 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	loop_heredoc(t_script *script, int pipe, int i)
{
	char	*tmp;
	char	*bis;

	bis = "";
	while (1)
	{
		tmp = readline("> ");
		if (!ft_strncmp(tmp, script->commands[i].in.name,
				ft_strlen(script->commands[i].in.name) + 1))
			break ;
		tmp = ft_strjoin(tmp, "\n");
		bis = ft_strjoin(bis, tmp);
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
