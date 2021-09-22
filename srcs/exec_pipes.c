#include "minishell.h"

static void	middle_child(t_script script, char **path_env, int *pipein, int *pipeout, int i)
{
	int ret;

	ret = check_builtin(script.commands[i].argv[0]);
	if (script.commands[i].in.name)
		pipe_in_redir(script, i);
	else 
	{
		if (dup2(pipein[0], STDIN_FILENO) == -1)
		{
			write(2, "Error: dup2 failed\n", 19);
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	if (script.commands[i].out.name)
		pipe_out_redir(script, i);
	else
	{
		if (dup2(pipeout[1], STDOUT_FILENO) == -1)
		{
			write(2, "Error: dup2 failed\n", 19);
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	close(pipein[1]);
	close(pipeout[0]);
	if (!ret)
	{
		exec_cmd(path_env, script.commands[i].argv, script.envp);
		write(2, "command not found\n", 18);
	}
	else
	{
		handle_builtin(ret, script, i);
		exit(0);
	}
	//free etc
}

static int	middle_cmds(t_script script, char **path_env, int *pipein, int *pipeout, int i)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (1); //error
	if (pid == 0)
		middle_child(script, path_env, pipein, pipeout, i);
	close(pipein[0]);
	close(pipeout[1]);
	wait(0);
	return (0);
}

static int	middle_loop(t_script script, char **path_env, int *pipe1, int *pipe2)
{
	int	i;
	int	check;

	i = 1;
	check = 0;
	while (i < script.cmd_count - 1)
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

static void	first_child(t_script script, char **path_env, int *pipe1)
{
	int ret;

	ret = check_builtin(script.commands[0].argv[0]);
	if (script.commands[0].in.name)
		pipe_in_redir(script, 0);
	if (script.commands[0].out.name)
		pipe_out_redir(script, 0);
	else
	{
		if (pipe1[1] != STDOUT_FILENO)
		{
			if (dup2(pipe1[1], STDOUT_FILENO) == -1)
			{
				//free tout ce qu'il faut + exec_status = 1 ou 126
				exit(1);
			}
		}
	}
	close(pipe1[0]);
	close(pipe1[1]);
	if (!ret)
	{
		exec_cmd(path_env, script.commands[0].argv, script.envp);
		write(2, "command not found\n", 18);
	}
	else
	{
		handle_builtin(ret, script, 0);
		exit(0);
	}
	//free etc
}

static void	last_child(t_script script, char **path_env, int *pipein, int i)
{
	int ret;

	ret = check_builtin(script.commands[i].argv[0]);
	if (script.commands[i].in.name)
		pipe_in_redir(script, i);
	else
	{
		if (pipein[0] != STDIN_FILENO)
		{
			if (dup2(pipein[0], STDIN_FILENO) == -1)
			{
				write(2, "Error: dup2 failed\n", 19);
				//free tout ce qu'il faut + exec_status = 1 ou 126
				exit(1);
			}
		}
	}
	if (script.commands[i].out.name)
		pipe_out_redir(script, i);
	close(pipein[1]);
	close(pipein[0]);
	if (!ret)
	{
		exec_cmd(path_env, script.commands[i].argv, script.envp);
		write(2, "command not found\n", 18);
	}
	else
	{
		handle_builtin(ret, script, i);
		exit(0);
	}
	//free etc
}

static void	last_cmd(t_script script, char **path_env, int *pipein, int pid2)
{
	int	i;

	i = script.cmd_count - 1;
	if (pid2 == 0)
		last_child(script, path_env, pipein, i);
	//end of function free everything etc
}

int	pipex(t_script script, char **path_env)
{
	int	pipe1[2];
	int	pipe2[2];
	int	check;
	int	pid1;
	int	pid2;

	check = 0;
	if (pipe(pipe1) == -1)
		return (1); //error
	pid1 = fork();
	if (pid1 == -1)
		return (1); //error
	if (pid1 == 0)
		first_child(script, path_env, pipe1);
	close(pipe1[1]);
	wait(0);
	check = middle_loop(script, path_env, pipe1, pipe2);
	if (check == -1)
		return (1); //error
	pid2 = fork();
	if (pid2 == -1)
		return (1); //error
	if (check == 1)
		last_cmd(script, path_env, pipe2, pid2);
	else if (check == 0)
		last_cmd(script, path_env, pipe1, pid2);
	wait(0);
	return (0);
}