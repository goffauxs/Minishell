/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_onecmd_pipex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:45 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 15:38:27 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	one_cmd_exec(t_script *script, char **path_env)
{
	int	pid;

	if (script->commands[0].in.flag == -1)
		signal(SIGQUIT, SIG_IGN);
	else
		signal(SIGQUIT, sig_handler_fork);
	signal(SIGINT, sig_handler_fork);
	pid = fork();
	if (pid == -1)
		return (fork_error(path_env));
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
	if (ret == 2 || ret == 5
		|| (ret == 4 && script->commands[0].argv[1]) || ret == 7)
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
		{
			free_path_env(path_env);
			return (1);
		}
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
	signal(SIGINT, sig_handler_fork);
	if (first_cmd(script, path_env, pipe1) == 1)
		return (1);
	check = mid_loop(script, path_env, pipe1, pipe2);
	if (check == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (fork_error(path_env));
	if (check % 2 == 1)
		last_cmd(script, path_env, pipe2, pid);
	else
		last_cmd(script, path_env, pipe1, pid);
	wait(&g_exit_status);
	while (check-- > 0)
		wait(&g_exit_status);
	wait(&g_exit_status);
	return (0);
}
