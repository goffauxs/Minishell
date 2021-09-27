#include "minishell.h"

void	sig_handler(int signum)
{
	if (signum == SIGQUIT && g_pid != 0)
	{
		write(1, "Quit: 3\n", 8);
		rl_on_new_line();
		tputs(carriage_return, 1, ft_putchar);
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
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
