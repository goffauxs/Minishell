/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:45 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 19:15:55 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd( char **path, char **cmd, char **env)
{
	char	*tmp;
	int		i;

	tmp = ft_strdup(*cmd);
	i = 0;
	if (execve(*cmd, cmd, env) != -1)
	{
		free(tmp);
		return ;
	}
	while (execve(tmp, cmd, env) == -1 && path[i])
	{
		free(tmp);
		tmp = ft_strjoin(path[i], *cmd);
		if (!tmp)
			break ;
		i++;
	}
	if (tmp)
		free(tmp);
}

int	handle_cmd(t_script *script)
{
	char	**path_env;

	script->termios_p.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &script->termios_p);
	path_env = split_paths(script->envp);
	if (script->cmd_count == 1)
	{
		if (one_cmd(script, path_env))
			return (1);
	}
	else
		if (pipex(script, path_env))
			return (1);
	script->termios_p.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &script->termios_p);
	return (0);
}

int	check_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5))
		if (ft_strncmp(cmd, "cd", 3))
			if (ft_strncmp(cmd, "pwd", 4))
				if (ft_strncmp(cmd, "export", 7))
					if (ft_strncmp(cmd, "unset", 6))
						if (ft_strncmp(cmd, "env", 4))
							if (ft_strncmp(cmd, "exit", 5))
								return (0);
	else
		return (7);
	else
		return (6);
	else
		return (5);
	else
		return (4);
	else
		return (3);
	else
		return (2);
	else
		return (1);
}

int	handle_builtin(int ret, t_script *script, int i)
{
	if (ret == 1)
		g_exit_status = builtin_echo(script->commands[i]);
	if (ret == 2)
		g_exit_status = builtin_cd(script->commands[i], script->envp);
	if (ret == 3)
		g_exit_status = builtin_pwd(script->envp);
	if (ret == 4)
		g_exit_status = builtin_export(script, script->commands[i]);
	if (ret == 5)
		g_exit_status = builtin_unset(script, script->commands[i]);
	if (ret == 6)
		g_exit_status = builtin_env(script->envp, script->commands[i]);
	if (ret == 7)
		return (builtin_exit(script->commands[i], script->cmd_count));
	return (0);
}
