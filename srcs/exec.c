#include "minishell.h"

static char	*add_forw_slash(char *str)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strdup(str);
	free(str);
	str = (char *)malloc(sizeof(char) * (ft_strlen(tmp) + 2));
	while (tmp[i])
	{
		str[i] = tmp[i];
		i++;
	}
	str[i++] = '/';
	str[i] = '\0';
	free(tmp);
	return (str);
}

static int	check_path_line(char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	return (i);
}

static void	init_vars(int *i, int *j)
{
	*i = 0;
	*j = 5;
}

static char	**split_paths(char **env)
{
	char	**path;
	char	*tmp;
	int		i;
	int		j;
	int		p_line;

	init_vars(&i, &j);
	p_line = check_path_line(env);
	path = ft_split(env[p_line], ':');
	tmp = ft_strdup(path[0]);
	free(path[0]);
	path[0] = (char *)malloc(sizeof(char) * ((ft_strlen(tmp) - 5) + 1));
	while (tmp[j])
		path[0][i++] = tmp[j++];
	path[0][i] = '\0';
	free(tmp);
	i = 0;
	while (path[i])
	{
		tmp = ft_strdup(path[i]);
		free(path[i]);
		path[i] = add_forw_slash(tmp);
		i++;
	}
	return (path);
}
//
//
static void	exec_cmd( char **path, char **cmd, char **env)
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

void	child(char **path_env, t_script script, int i)
{
	char *backup;
	backup = ft_strdup(script.commands[i].cmd);
	exec_cmd(path_env, script.commands[i].argv, script.envp);
	printf("%s: command not found\n", backup);
	//free etc
}

void	handle_cmd(t_script script, int i)
{
	char	**path_env;
	int		pid;

	path_env = split_paths(script.envp);
	pid = fork();
	if (pid == -1)
	{
		exit_status = 1 ;
		return; //error
	}
	if (pid == 0)
		child(path_env, script, i);
	waitpid(0, &exit_status, 0);
	free(path_env);
	if (exit_status == 256 || exit_status == 512)
		exit_status /= 256;
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

int	handle_builtin(int ret, t_script script, int i)
{
	if (ret == 1)
		exit_status = builtin_echo(script.commands[i]); // ok
	if (ret == 2)
		exit_status = builtin_cd(script.commands[i]); // ok
	if (ret == 3)
		exit_status = builtin_pwd(); // ok
	if(ret == 4)
		exit_status = builtin_export(&script); // segfault
	if (ret == 6)
		exit_status = builtin_env(script.envp); // ok
	if(ret == 7)
		return(builtin_exit(script.commands[i])); // ok
	return(0);
}