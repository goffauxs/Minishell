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
		in_redir(script, 0, path_env);
	if (script->commands[0].out.name)
		out_redir(script, 0, path_env);
	else if (pipe1)
	{
		if (pipe_dup(pipe1, 1, STDOUT_FILENO) == 1)
		{
			free_cmds_path(script, path_env);
			close_pipes(pipe1, NULL);
			exit(1);
		}
		close_pipes(pipe1, NULL);
	}
	if (!nocmd)
		cmd_builtin(script, path_env, ret, 0);
	free_cmds_path(script, path_env);
	exit(0);
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
		in_redir(script, i, path_env);
	else if (pipe_dup(pipein, 0, STDIN_FILENO) == 1)
	{
		close_pipes(pipein, pipeout);
		free_cmds_path(script, path_env);
		exit(1);
	}
	if (script->commands[i].out.name)
		out_redir(script, i, path_env);
	else if (pipe_dup(pipeout, 1, STDOUT_FILENO) == 1)
	{
		close_pipes(pipein, pipeout);
		free_cmds_path(script, path_env);
		exit(1);
	}
	close_pipes(pipein, pipeout);
	if (!nocmd)
		cmd_builtin(script, path_env, ret, i);
	free_cmds_path(script, path_env);
	exit(0);
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
		in_redir(script, i, path_env);
	else if (pipe_dup(pipein, 0, STDIN_FILENO) == 1)
	{
		close_pipes(pipein, NULL);
		free_cmds_path(script, path_env);
		exit(1);
	}
	if (script->commands[i].out.name)
		out_redir(script, i, path_env);
	close_pipes(pipein, NULL);
	if (!nocmd)
		cmd_builtin(script, path_env, ret, i);
	free_cmds_path(script, path_env);
	exit(0);
}
