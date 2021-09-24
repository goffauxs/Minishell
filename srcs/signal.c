#include "minishell.h"

void	sig_handler(int signum)
{
	if(signum == SIGINT && g_pid != 0)
	{
		rl_on_new_line();
	}
	else if (signum == SIGQUIT && g_pid != 0)
	{
		write(1, "Quit: 3\n", 8);
		rl_on_new_line();
	}
	else if (g_pid == 0)
	{
		if(signum == SIGINT) // ctrl c
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (signum == SIGQUIT) // ctrl backslash
		{
			rl_redisplay();
		}
	}
}

