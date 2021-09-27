#include "minishell.h"

void	heredoc(t_script *script, int i, char **path_env)
{
	char	*tmp;
	char	*bis;
	int		pipe_tmp[2];

	bis = "";
	if (pipe(pipe_tmp) == -1)
	{
		write(2, "Error: pipe failed\n", 19);
		free_cmds_path(script, path_env);
		exit(1);
	}
	while (1)
	{
		tmp = readline("> ");
		if (!ft_strncmp(tmp, script->commands[i].in.name,
				ft_strlen(script->commands[i].in.name) + 1))
			break ;
		tmp = ft_strjoin(tmp, "\n");
		bis = ft_strjoin(bis, tmp);
	}
	write(pipe_tmp[1], bis, ft_strlen(bis));
	if (pipe_tmp[0] != STDIN_FILENO)
	{
		if (dup2(pipe_tmp[0], STDIN_FILENO) == -1)
		{
			write(2, "Error: dup2 failed\n", 19);
			close_pipes(pipe_tmp, NULL);
			free_cmds_path(script, path_env);
			exit(1);
		}
	}
	close(pipe_tmp[0]);
	close(pipe_tmp[1]);
}
