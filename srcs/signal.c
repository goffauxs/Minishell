#include "minishell.h"

void	sig_handler(int signum)
{
	struct termios termios_p;
	tcgetattr(STDIN_FILENO, &termios_p);
	if (signum == SIGQUIT && g_pid != 0)
	{
		write(1, "Quit: 3\n", 8);
		termios_p.c_lflag |= ECHO;
		rl_on_new_line();
	}
	else if (signum == SIGINT && g_pid != 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
	}
	else if (signum == SIGINT && g_pid == 0)
	{
		rl_on_new_line();
		write(1, "\n", 1);
		rl_redisplay();
	}
}
