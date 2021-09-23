#include "minishell.h"

void	exec_cmd( char **path, char **cmd, char **env)
{
	char	*tmp;
	int		i;

	tmp = ft_strdup(*cmd);
	i = 0;
	while (execve(*cmd, cmd, env) == -1 && path[i])
	{
		free(*cmd);
		*cmd = ft_strjoin(path[i], tmp);
		if (!*cmd)
		{
			free(cmd);
			break ;
		}
		i++;
	}
}

void	handle_cmd(t_script *script)
{
	char	**path_env;

	path_env = split_paths(script->envp);
	if (script->cmd_count == 1)
	{
		g_pid = fork();
		if (g_pid == -1)
		{
			script->exit_status = 1;
			return ; //error
		}
		if (g_pid == 0)
			first_child(script, path_env, NULL);
		waitpid(0, &script->exit_status, 0);
		if (script->exit_status == 256 || script->exit_status == 512)
			script->exit_status /= 256;
	}
	else
		pipex(script, path_env);
	free(path_env);
}

int	check_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4))
		if (ft_strncmp(cmd, "cd", 2))
			if (ft_strncmp(cmd, "pwd", 3))
				if (ft_strncmp(cmd, "export", 6))
					if (ft_strncmp(cmd, "unset", 5))
						if (ft_strncmp(cmd, "env", 3))
							if (ft_strncmp(cmd, "exit", 4))
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
		script->exit_status = builtin_echo(script->commands[i]); // ok
	if (ret == 2)
		script->exit_status = builtin_cd(script->commands[i]); // ok
	if (ret == 3)
		script->exit_status = builtin_pwd(); // ok
	if (ret == 4)
		script->exit_status = builtin_export(&script->envp, script->commands[i]); // segfault
	if (ret == 6)
		script->exit_status = builtin_env(script->envp); // ok
	if (ret == 7)
		return (builtin_exit(script->commands[i], script)); // ok
	return (0);
}
