#include "minishell.h"
#include "libft.h"

void	sig_handler(int signum)
{
	if(signum == SIGINT)
		exit(EXIT_SUCCESS);
	//if(signum == SIGQUIT)
		// FAIS DE LA MAGIE NOIRE ?????????
}