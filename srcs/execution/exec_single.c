#include "minishell.h"

static void	single_child(char **path_env, t_script *script)
{
	int ret;

	ret = check_builtin(script->commands[0].argv[0]);
	if (script->commands[0].in.name)
		in_redir(script, 0);
	if (script->commands[0].out.name)
		out_redir(script, 0);
	if(!ret)
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

void	single_cmd(t_script *script, char **path_env)
{
	int	pid;
	int	ret;

	ret = check_builtin(script->commands[0].argv[0]);
	if(ret == 0)
	{
		pid = fork();
		if (pid == -1)
		{
			exit_status = 1 ;
			return ; //error
		}
		if (pid == 0)
			single_child(path_env, script);
		waitpid(0, &exit_status, 0);
		if (exit_status == 256 || exit_status == 512)
			exit_status /= 256;
	}
	else
	{
		if (handle_builtin(ret, script, 0))
			ret ++; // condition for the exit : return 1 only if exit is needed
	}
}
