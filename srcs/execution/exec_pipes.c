#include "minishell.h"

static int	middle_cmds(t_script *script, char **path_env, int *pipein, int *pipeout, int i)
{
	g_pid = fork();
	if (g_pid == -1)
		return (1); //error
	if (g_pid == 0)
		middle_child(script, path_env, pipein, pipeout, i);
	close(pipein[0]);
	close(pipeout[1]);
	wait(0);
	return (0);
}

static int	middle_loop(t_script *script, char **path_env, int *pipe1, int *pipe2)
{
	int	i;
	int	check;

	i = 1;
	check = 0;
	while (i < script->cmd_count - 1)
	{
		if (check == 0)
		{
			if (pipe(pipe2) == -1)
				return (-1); //error
			middle_cmds(script, path_env, pipe1, pipe2, i);
			check = 1;
		}
		else if (check == 1)
		{
			if (pipe(pipe1) == -1)
				return (-1); //error
			middle_cmds(script, path_env, pipe2, pipe1, i);
			check = 0;
		}
		i++;
	}
	return (check);
}

static void	last_cmd(t_script *script, char **path_env, int *pipein)
{
	int	i;

	i = script->cmd_count - 1;
	if (g_pid == 0)
		last_child(script, path_env, pipein, i);
	//end of function free everything etc
}

int	pipex(t_script *script, char **path_env)
{
	int	pipe1[2];
	int	pipe2[2];
	int	check;

	check = 0;
	if (pipe(pipe1) == -1)
		return (1); //error
	g_pid = fork();
	if (g_pid == -1)
		return (1); //error
	if (g_pid == 0)
		first_child(script, path_env, pipe1);
	close(pipe1[1]);
	wait(0);
	check = middle_loop(script, path_env, pipe1, pipe2);
	if (check == -1)
		return (1); //error
	g_pid = fork();
	if (g_pid == -1)
		return (1); //error
	if (check == 1)
		last_cmd(script, path_env, pipe2);
	else if (check == 0)
		last_cmd(script, path_env, pipe1);
	wait(0);
	return (0);
}