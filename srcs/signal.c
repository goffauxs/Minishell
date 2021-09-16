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
