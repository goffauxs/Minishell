/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_onecmd_pipex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:45 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/29 10:02:30 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	one_cmd_exec(t_script *script, char **path_env)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		fork_error(path_env);
		return (1);
	}
	if (pid == 0)
		first_child(script, path_env, NULL);
	wait(&g_exit_status);
	return (0);
}

int	one_cmd(t_script *script, char **path_env)
{
	int	ret;

	ret = 0;
	if (script->commands[0].argv[0])
		ret = check_builtin(script->commands[0].argv[0]);
	if (ret > 0)
	{
		if (handle_builtin(ret, script, 0))
		{
			free_path_env(path_env);
			return (1);
		}
	}
	else
	{
		if (one_cmd_exec(script, path_env))
			return (1);
	}
	free_path_env(path_env);
	return (0);
}

int	pipex(t_script *script, char **path_env)
{
	int	pid;
	int	pipe1[2];
	int	pipe2[2];
	int	check;

	check = 0;
	if (first_cmd(script, path_env, pipe1) == 1)
		return (1);
	wait(&g_exit_status);
	check = mid_loop(script, path_env, pipe1, pipe2);
	if (check == -1)
		return (1);
	pid = fork();
	if (pid == -1)
	{
		fork_error(path_env);
		return (1);
	}
	if (check == 1)
		last_cmd(script, path_env, pipe2, pid);
	else if (check == 0)
		last_cmd(script, path_env, pipe1, pid);
	wait(&g_exit_status);
	return (0);
}
