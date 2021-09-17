#include "minishell.h"

void	sig_handler(int signum)
{
	char	*prompt;
	size_t	i;

	if(signum == SIGINT)
	{
		i = 0;
		prompt = get_prompt();
		// CONDITION SI PROCESS QUI TOURNE PAS
		// while (i < ft_strlen(prompt) + 2)
		// {
		// 	write(1, " \b", 2);
		// 	if (i < ft_strlen(prompt))
		// 		write(1, &prompt[i], 1);
		// 	else if (i == ft_strlen(prompt))
		// 		write(1, " ", 1);
		// 	i++;
		// }
		write(1, "\n", 1);
		// SI PROCESS QUI TOURNE PAS 
		//write(1, prompt, ft_strlen(prompt));
		signal(SIGINT, sig_handler);
	}
	// else if (signum == SIGQUIT)
	// {
	// 	tourne pas : rien // signal(SIGINT, SIG_IGN);
	// 	tourne : quit 3;
	// }
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

