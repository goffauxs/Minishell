#include "minishell.h"

void	sig_handler(int signum)
{
	char	*prompt;

	prompt = get_prompt();
	if((signum == SIGINT || signum == SIGQUIT) && glo.running_pid != 0)
	{
		if(!kill(glo.running_pid, 0))
		{
			if(signum == SIGINT) // ctrl c OK
			{
				write(1, "\n", 1);
				glo.exit_status = 131;
			}
			if(signum == SIGQUIT) // ctrl backslash 
			{
				write(1,"Quit: 3\n",8);
				glo.exit_status = 130;
			}
		}
	}
	else if (glo.running_pid == 0)
	{
		if(signum == SIGINT) // ctrl c
		{
			glo.exit_status = 1;
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

//int kill(pid_t pid, int sig);
// If sig is 0, then no signal is sent, but existence and permission
//        checks are still performed; this can be used to check for the
//        existence of a process ID or process group ID that the caller is
//        permitted to signal.

// RETURN VALUE         top

//        On success (at least one signal was sent), zero is returned.  On
//        error, -1 is returned, and errno is set to indicate the error.

//  ESRCH  The target process or process group does not exist.  Note
//               that an existing process might be a zombie, a process that
//               has terminated execution, but has not yet been wait(2)ed
//               for.

