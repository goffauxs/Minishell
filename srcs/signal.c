#include "minishell.h"

void	sig_handler(int signum)
{
	if((signum == SIGINT || signum == SIGQUIT) && g_pid != 0)
	{
		// if(!kill(g_pid, signum))
		// {
		// 	if(signum == SIGINT) // ctrl c OK
		// 	{
		// 		write(1, "\n", 1);
		// 		// glo.exit_status = 131;
		// 	}
		// 	if(signum == SIGQUIT) // ctrl backslash 
		// 	{
		// 		write(1,"Quit: 3\n",8);
		// 		// glo.exit_status = 130;
		// 	}
		// }
		kill(g_pid, 0);
		write(1,"\n",1);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (g_pid == 0)
	{
		if(signum == SIGINT) // ctrl c
		{
			// glo.exit_status = 1;
			write(1,"\n",1);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (signum == SIGQUIT) // ctrl backslash
		{
			signal(SIGQUIT, SIG_IGN);
		}
	}
}

