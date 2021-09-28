/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_onecmd_pipex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:45 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/28 14:33:22 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	one_cmd_exec(t_script *script, char **path_env)
{
	g_pid = fork();
	if (g_pid == -1)
	{
		fork_error(script, path_env);
		return (1);
	}
	if (g_pid == 0)
		first_child(script, path_env, NULL);
	wait(&script->exit_status);
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
	int	pipe1[2];
	int	pipe2[2];
	int	check;

	check = 0;
	if (first_cmd(script, path_env, pipe1) == 1)
		return (1);
	wait(&script->exit_status);
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
	wait(&script->exit_status);
	return (0);
}
