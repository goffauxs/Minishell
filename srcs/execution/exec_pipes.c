/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:36 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/30 14:38:42 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	first_cmd(t_script *script, char **path_env, int *pipe1)
{
	int	pid;

	if (pipe(pipe1) == -1)
		return (pipe_error(path_env));
	pid = fork();
	if (pid == -1)
		return (fork_error(path_env));
	if (pid == 0)
		first_child(script, path_env, pipe1);
	close(pipe1[1]);
	return (0);
}

static int	middle_cmds(t_script *script, char **path_env, int **pipes, int i)
{
	int	pid;

	if (!pipes)
		return (1);
	pid = fork();
	if (pid == -1)
		return (fork_error(path_env));
	if (pid == 0)
		middle_child(script, path_env, pipes, i);
	close(pipes[0][0]);
	close(pipes[1][1]);
	free(pipes);
	return (0);
}

static int	**pipe_init(char **path_env, int *pipe1, int *pipe2)
{
	int	**pipes;

	pipes = malloc(sizeof(int *) * 2);
	if (!pipes)
		return (NULL);
	if (!pipes)
		return (NULL);
	if (pipe(pipe2) == -1)
	{
		pipe_error(path_env);
		return (NULL);
	}
	pipes[0] = pipe1;
	pipes[1] = pipe2;
	return (pipes);
}

int	mid_loop(t_script *s, char **path_env, int *pipe1, int *pipe2)
{
	int	i;
	int	check;
	int	**pipes;

	i = 0;
	check = 0;
	while (++i < s->cmd_count - 1)
	{
		if (check % 2 == 0)
		{
			pipes = pipe_init(path_env, pipe1, pipe2);
			if (middle_cmds(s, path_env, pipes, i) == 1)
				return (-1);
		}
		else
		{
			pipes = pipe_init(path_env, pipe2, pipe1);
			if (middle_cmds(s, path_env, pipes, i) == 1)
				return (-1);
		}
		check++;
	}
	return (check);
}

void	last_cmd(t_script *s, char **path_env, int *pipein, int pid)
{
	int	i;

	i = s->cmd_count - 1;
	if (pid == 0)
		last_child(s, path_env, pipein, i);
	free_path_env(path_env);
}
