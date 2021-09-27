/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:36 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/27 16:30:24 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	middle_cmds(t_script *script, char **path_env, int *pipein, int *pipeout, int i)
{
	g_pid = fork();
	if (g_pid == -1)
	{
		fork_error(script, path_env);
		return (1);
	}
	if (g_pid == 0)
		middle_child(script, path_env, pipein, pipeout, i);
	close(pipein[0]);
	close(pipeout[1]);
	wait(0);
	return (0);
}

static int	middle_loop(t_script *script, char **path_env, int *pipe1, int *pipe2)
{
	int	i;
	int	check;

	i = 1;
	check = 0;
	while (i < script->cmd_count - 1)
	{
		if (check == 0)
		{
			if (pipe(pipe2) == -1)
			{
				pipe_error(script, path_env);
				return (-1);
			}
			middle_cmds(script, path_env, pipe1, pipe2, i);
			check = 1;
		}
		else if (check == 1)
		{
			if (pipe(pipe1) == -1)
			{
				pipe_error(script, path_env);
				return (-1);
			}
			middle_cmds(script, path_env, pipe2, pipe1, i);
			check = 0;
		}
		i++;
	}
	return (check);
}

static void	last_cmd(t_script *script, char **path_env, int *pipein)
{
	int	i;

	i = script->cmd_count - 1;
	if (g_pid == 0)
		last_child(script, path_env, pipein, i);
	free_path_env(path_env);
}

int	pipex(t_script *script, char **path_env)
{
	int	pipe1[2];
	int	pipe2[2];
	int	check;

	check = 0;
	if (pipe(pipe1) == -1)
	{
		pipe_error(script, path_env);
		return (1);
	}
	g_pid = fork();
	if (g_pid == -1)
	{
		fork_error(script, path_env);
		return (1);
	}
	if (g_pid == 0)
		first_child(script, path_env, pipe1);
	close(pipe1[1]);
	wait(0);
	check = middle_loop(script, path_env, pipe1, pipe2);
	if (check == -1)
		return (1);
	g_pid = fork();
	if (g_pid == -1)
	{
		fork_error(script, path_env);
		return (1);
	}
	if (check == 1)
		last_cmd(script, path_env, pipe2);
	else if (check == 0)
		last_cmd(script, path_env, pipe1);
	wait(0);
	return (0);
}
