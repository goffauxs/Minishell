#include "minishell.h"

void	pipe_in_redir(t_script script, int i)
{
	int	fdin;

	if (script.commands[i].in.flag >= 0)
	{
		fdin = open(script.commands[i].in.name, script.commands[i].in.flag);

		if(fdin == -1)
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