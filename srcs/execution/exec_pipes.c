/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:36 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/27 18:13:08 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	middle_cmds(t_script *script, char **path_env, int **pipes, int i)
{
	g_pid = fork();
	if (g_pid == -1)
	{
		fork_error(script, path_env);
		return (1);
	}
	if (g_pid == 0)
		middle_child(script, path_env, pipes, i);
	close(pipes[0][0]);
	close(pipes[1][1]);
	wait(0);
	return (0);
}

static int	**pipe_init(t_script *s, char **path_env, int *pipe1, int *pipe2)
{
	int	**pipes;

	pipes = malloc(sizeof(int *) * 2);
	if (pipe(pipe2) == -1)
	{
		pipe_error(s, path_env);
		return (NULL);
	}
	pipes[0] = pipe1;
	pipes[1] = pipe2;
	return (pipes);
}

static int	mid_loop(t_script *script, char **path_env, int *pipe1, int *pipe2)
{
	int	i;
	int	check;
	int	**pipes;

	i = 1;
	check = 0;
	while (i < script->cmd_count - 1)
	{
		if (check == 0)
		{
			pipes = pipe_init(script, path_env, pipe1, pipe2);
			if (!pipes)
				return (-1);
			middle_cmds(script, path_env, pipes, i);
			check = 1;
		}
		else if (check == 1)
		{
			pipes = pipe_init(script, path_env, pipe2, pipe1);
			if (!pipes)
				return (-1);
			middle_cmds(script, path_env, pipes, i);
			check = 0;
		}
		free(pipes);
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
	check = mid_loop(script, path_env, pipe1, pipe2);
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
