#include "minishell.h"

void	here_doc(t_script script, int i)
{
	char	*tmp;
	char	*bis;
	int		pipe_tmp[2];

	bis = "";
	pipe(pipe_tmp);
	while (1)
	{
		tmp = readline("> ");
		if (!ft_strncmp(tmp, script.commands[i].in.name,
				ft_strlen(script.commands[i].in.name) + 1))
			break ;
		tmp = ft_strjoin(tmp, "\n");
		bis = ft_strjoin(bis, tmp);
	}
	write(pipe_tmp[1], bis, ft_strlen(bis));
	dup2(pipe_tmp[0], STDIN_FILENO);
	close(pipe_tmp[1]);
	close(pipe_tmp[0]);
}
