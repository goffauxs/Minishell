/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:45 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/27 17:10:20 by mdeclerf         ###   ########.fr       */
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
	waitpid(0, &script->exit_status, 0);
	if (script->exit_status == 256 || script->exit_status == 512)
		script->exit_status /= 256;
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
