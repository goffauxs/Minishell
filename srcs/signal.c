#include "minishell.h"

void	sig_handler(int signum)
{
	char	*prompt;
	size_t	i;

	i = 0;
	(void)signum;
	prompt = get_prompt();
	while (i < ft_strlen(prompt) + 2)
	{
		write(1, " \b", 2);
		if (i < ft_strlen(prompt))
			write(1, &prompt[i], 1);
		else if (i == ft_strlen(prompt))
			write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
	write(1, prompt, ft_strlen(prompt));
	signal(SIGINT, sig_handler);
}
