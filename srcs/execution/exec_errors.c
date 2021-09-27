#include "minishell.h"

void	fork_error(t_script *script, char **path_env)
{
	write(2, "Error: fork failed\n", 19);
	script->exit_status = 1;
	free_path_env(path_env);
}

void	pipe_error(t_script *script, char **path_env)
{
	write(2, "Error: pipe failed\n", 19);
	script->exit_status = 1;
	free_path_env(path_env);
}
