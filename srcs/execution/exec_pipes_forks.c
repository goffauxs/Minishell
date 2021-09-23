#include "minishell.h"

void	first_child(t_script *script, char **path_env, int *pipe1)
{
	int ret;

	ret = check_builtin(script->commands[0].argv[0]);
	if (script->commands[0].in.name)
		in_redir(script, 0);
	if (script->commands[0].out.name)
		out_redir(script, 0);
	else if (pipe1)
	{
		pipe_dup(pipe1, 1, STDOUT_FILENO);
		close_pipes(pipe1, NULL);
	}
	if (!ret)
	{
		exec_cmd(path_env, script->commands[0].argv, script->envp);
		write(2, "command not found\n", 18);
	}
	else
	{
		handle_builtin(ret, script, 0);
		exit(0);
	}
	//free etc
}

void	middle_child(t_script *script, char **path_env, int *pipein, int *pipeout, int i)
{
	int ret;

	ret = check_builtin(script->commands[i].argv[0]);
	if (script->commands[i].in.name)
		in_redir(script, i);
	else
		pipe_dup(pipein, 0, STDIN_FILENO);
	if (script->commands[i].out.name)
		out_redir(script, i);
	else
		pipe_dup(pipeout, 1, STDOUT_FILENO);
	close_pipes(pipein, pipeout);
	if (!ret)
	{
		exec_cmd(path_env, script->commands[i].argv, script->envp);
		write(2, "command not found\n", 18);
	}
	else
	{
		handle_builtin(ret, script, i);
		exit(0);
	}
	//free etc
}

void	last_child(t_script *script, char **path_env, int *pipein, int i)
{
	int ret;

	ret = check_builtin(script->commands[i].argv[0]);
	if (script->commands[i].in.name)
		in_redir(script, i);
	else
		pipe_dup(pipein, 0, STDIN_FILENO);
	if (script->commands[i].out.name)
		out_redir(script, i);
	close_pipes(pipein, NULL);
	if (!ret)
	{
		exec_cmd(path_env, script->commands[i].argv, script->envp);
		write(2, "command not found\n", 18);
	}
	else
	{
		handle_builtin(ret, script, i);
		exit(0);
	}
	//free etc
}