#include "minishell.h"

void	in_redir(t_script *script, int i)
{
	int	fdin;

	if (script->commands[i].in.flag >= 0)
	{
		fdin = open(script->commands[i].in.name, script->commands[i].in.flag);
		if (fdin == -1)
		{
			printf("%s: No such file or directory\n", script->commands[i].in.name);
			close(fdin);
			exit(1);
		}
		else if (fdin != STDIN_FILENO)
			dup2(fdin, STDIN_FILENO);
	}
	else
		here_doc(script, i);
}

void	out_redir(t_script *script, int i)
{
	int	fdout;

	fdout = open(script->commands[i].out.name, script->commands[i].out.flag, 0644);
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
