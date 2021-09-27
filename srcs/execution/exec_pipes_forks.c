#include "minishell.h"

void	first_child(t_script *script, char **path_env, int *pipe1)
{
	int	ret;
	int	nocmd;

	ret = 0;
	nocmd = 1;
	if (script->commands[0].argv[0])
	{
		ret = check_builtin(script->commands[0].argv[0]);
		nocmd = 0;
	}
	if (script->commands[0].in.name)
		in_redir(script, 0);
	if (script->commands[0].out.name)
		out_redir(script, 0);
	else if (pipe1)
	{
		pipe_dup(pipe1, 1, STDOUT_FILENO);
		close_pipes(pipe1, NULL);
	}
	if (!nocmd)
		cmd_builtin(script, path_env, ret, 0);
	exit(0);
	//free etc
}

void	middle_child(t_script *script, char **path_env, int *pipein, int *pipeout, int i)
{
	int	ret;
	int	nocmd;

	ret = 0;
	nocmd = 1;
	if (script->commands[i].argv[0])
	{
		ret = check_builtin(script->commands[i].argv[0]);
		nocmd = 0;
	}
	if (script->commands[i].in.name)
		in_redir(script, i);
	else
		pipe_dup(pipein, 0, STDIN_FILENO);
	if (script->commands[i].out.name)
		out_redir(script, i);
	else
		pipe_dup(pipeout, 1, STDOUT_FILENO);
	close_pipes(pipein, pipeout);
	if (!nocmd)
		cmd_builtin(script, path_env, ret, i);
	exit(0);
	//free etc
}

void	last_child(t_script *script, char **path_env, int *pipein, int i)
{
	int	ret;
	int	nocmd;

	ret = 0;
	nocmd = 1;
	if (script->commands[i].argv[0])
	{
		ret = check_builtin(script->commands[i].argv[0]);
		nocmd = 0;
	}
	if (script->commands[i].in.name)
		in_redir(script, i);
	else
		pipe_dup(pipein, 0, STDIN_FILENO);
	if (script->commands[i].out.name)
		out_redir(script, i);
	close_pipes(pipein, NULL);
	if (!nocmd)
		cmd_builtin(script, path_env, ret, i);
	exit(0);
	//free etc
}