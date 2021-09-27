#include "minishell.h"

void	in_redir(t_script *s, int i)
{
	int	fdin;

	if (s->commands[i].in.flag >= 0)
	{
		fdin = open(s->commands[i].in.name, s->commands[i].in.flag);
		if (fdin == -1)
		{
			printf("%s: No such file or directory\n", s->commands[i].in.name);
			close(fdin);
			exit(1);
		}
		else if (fdin != STDIN_FILENO)
			dup2(fdin, STDIN_FILENO);
	}
	else
		heredoc(s, i);
}

void	out_redir(t_script *s, int i)
{
	int	fdout;

	fdout = open(s->commands[i].out.name, s->commands[i].out.flag, 0644);
	if (fdout != STDOUT_FILENO)
	{
		if (dup2(fdout, STDOUT_FILENO) == -1)
		{
			write(2, "Error: dup2 failed\n", 19);
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit (1);
		}
	}
	close(fdout);
}

void	close_pipes(int *pipe1, int *pipe2)
{
	close(pipe1[0]);
	close(pipe1[1]);
	if (pipe2)
	{
		close(pipe2[0]);
		close(pipe2[1]);
	}
}

void	pipe_dup(int *pipe, int mod, int std)
{
	if (pipe[mod] != std)
	{
		if (dup2(pipe[mod], std) == -1)
		{
			write(2, "Error: dup2 failed\n", 19);
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
}

void	cmd_builtin(t_script *script, char **path_env, int ret, int i)
{
	if (!ret)
	{
		exec_cmd(path_env, script->commands[i].argv, script->envp);
		write(2, "command not found\n", 18);
	}
	else
		handle_builtin(ret, script, i);
}


