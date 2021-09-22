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

static void	child(char **path_env, t_script script)
{
	// char *backup;
	// backup = ft_strdup(script.commands[0].argv[0]);
	// exec_cmd(path_env, script.commands[0].argv, script.envp);
	// printf("%s: command not found\n", backup);
	int ret;
	int fdin;
	int fdout;
	int	pi[2];

	ret = check_builtin(script.commands[0].argv[0]);
	if (script.commands[0].in.name)
	{
		if (script.commands[0].in.flag >= 0)
		{
			fdin = open(script.commands[0].in.name,  script.commands[0].in.flag);
			if(fdin != STDIN_FILENO)
				dup2(fdin, STDIN_FILENO);
			close(fdin);
		}
		else
		{
			pipe(pi);
			//here doc 
			char *tmp;
			char *bis;
			bis = "";
			while(1)
			{
				tmp = readline("> ");
				if(!ft_strncmp(tmp, script.commands[0].in.name, ft_strlen(tmp)))
					break ;
				tmp = ft_strjoin(tmp, "\n");
				bis = ft_strjoin(bis, tmp);
			}
			write(pi[1], bis, ft_strlen(bis));
			dup2(pi[0], STDIN_FILENO);
			close(pi[1]);
			close(pi[0]);
		}
	}
	if (script.commands[0].out.name)
	{
		printf("here\n");
		fdout = open(script.commands[0].out.name, script.commands[0].out.flag, 0644);
		//fdout = open(script.commands[0].out.name,script.commands[0].out.flag, 0644 );
		if(fdout != STDOUT_FILENO)
			dup2(fdout, STDOUT_FILENO);
		close(fdout);
	}
	if(!ret)
	{
		exec_cmd(path_env, script.commands[0].argv, script.envp);
		//exec_cmd(path_env, script.commands[0].argv, script.envp);
		write(2, "command not found\n", 18);
	}
	else
	{
		handle_builtin(ret, script, 0);
		exit(0);
	}
	//free etc
}

void	handle_cmd(t_script script)
{
	char	**path_env;
	int		pid;
	int		ret;

	ret = 0;
	path_env = split_paths(script.envp);
	if (script.cmd_count == 1)
	{
		ret = check_builtin(script.commands[0].argv[0]);
		if(ret == 0)
		{
			pid = fork();
			if (pid == -1)
			{
				exit_status = 1 ;
				return ; //error
			}
			if (pid == 0)
				child(path_env, script);
			waitpid(0, &exit_status, 0);
			if (exit_status == 256 || exit_status == 512)
				exit_status /= 256;
		}
		else
		{
			if (handle_builtin(ret, script, 0))
				ret ++; // condition for the exit : return 1 only if exit is needed
		}
	}
	else
	{
		pipex(script, path_env);
	}
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
