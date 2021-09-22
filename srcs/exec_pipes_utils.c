#include "minishell.h"

void	pipe_in_redir(t_script script, int i)
{
	int	fdin;

	if (script.commands[i].in.flag >= 0)
	{
		fdin = open(script.commands[i].in.name, script.commands[i].in.flag);
		if (fdin == -1)
		{
			printf("%s: No such file or directory\n", script.commands[i].in.name);
			close(fdin);
			exit(1);
		}
		else if (fdin != STDIN_FILENO)
			dup2(fdin, STDIN_FILENO);
	}
	else
		here_doc(script, i);
}

void	pipe_out_redir(t_script script, int i)
{
	int	fdout;

	fdout = open(script.commands[i].out.name, script.commands[i].out.flag, 0644);
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
