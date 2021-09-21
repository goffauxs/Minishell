#include "minishell.h"

static void	first_child(t_script script, char **path_env, int *pipe1)
{
	// int ret;
	//if (!script.commands.in)
	// |
	// v
	if (dup2(pipe1[1], STDOUT_FILENO) == -1)
	{
		//free tout ce qu'il faut + exec_status = 1 ou 126
		exit(1);
	}
	close(pipe1[0]);
	exec_cmd(path_env, script.commands[0].argv, script.envp);
	write(2, "command not found\n", 18);
	//free etc
}

static void	middle_child(t_script script, char **path_env, int *pipein, int *pipeout, int i)
{
	int	fdin;
	int	fdout;

	if (script.commands[i].in.name)
	{
		fdin = open(script.commands[i].in.name, O_RDONLY);
		if (dup2(fdin, STDIN_FILENO) == -1)
		{
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	else
	{
		if (dup2(pipein[0], STDIN_FILENO) == -1)
		{
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	if (script.commands[i].out.name)
	{
		fdout = open(script.commands[i].out.name, O_RDONLY);
		if (dup2(fdout, STDOUT_FILENO) == -1)
		{
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	else
	{
		if (dup2(pipeout[1], STDOUT_FILENO) == -1)
		{
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	close(pipein[1]);
	close(pipeout[0]);
	exec_cmd(path_env, script.commands[i].argv, script.envp);
	write(2, "command not found\n", 18);
	//free etc
}

static void	middle_builtin(t_script script, int *pipein, int *pipeout, int i, int ret)
{
	int	fdin;
	int	fdout;

	if (script.commands[i].in.name)
	{
		fdin = open(script.commands[i].in.name, O_RDONLY);
		if (dup2(fdin, STDIN_FILENO) == -1)
		{
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	else
	{
		if (dup2(pipein[0], STDIN_FILENO) == -1)
		{
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	if (script.commands[i].out.name)
	{
		fdout = open(script.commands[i].out.name, O_RDONLY);
		if (dup2(fdout, STDOUT_FILENO) == -1)
		{
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	else
	{
		if (dup2(pipeout[1], STDOUT_FILENO) == -1)
		{
			//free tout ce qu'il faut + exec_status = 1 ou 126
			exit(1);
		}
	}
	close(pipein[1]);
	close(pipeout[0]);
	handle_builtin(ret, script, 0);
	//free etc
}

static int	middle_cmds(t_script script, char **path_env, int *pipein, int *pipeout, int i)
{
	int	pid;
	int ret;

	ret = check_builtin(script.commands[i].cmd);
	pid = fork();
	if (pid == -1)
		return (1); //error
	if (pid == 0 && !ret)
		middle_child(script, path_env, pipein, pipeout, i);
	else if(pid && ret)
		middle_builtin(script, pipein, pipeout, i, ret);
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
			wait(0);
		}
		else if (check == 1)
		{
			if (pipe(pipe1) == -1)
				return (-1); //error
			middle_cmds(script, path_env, pipe2, pipe1, i);
			check = 0;
			wait(0);
		}
		i++;
	}
	return (check);
}

static void	last_child(t_script script, char **path_env, int *pipein, int i)
{
	//if (!script.commands[i].out)
	//	|
	//	v
	if (dup2(pipein[0], STDIN_FILENO) == -1)
	{
		perror(" Last child: ");
		write(2, "dup2 error\n", 11);
		//free tout ce qu'il faut + exec_status = 1 ou 126
		exit(1);
	}
	close(pipein[0]);
	close(pipein[1]);
	exec_cmd(path_env, script.commands[i].argv, script.envp);
	write(2, "command not found\n", 18);
	//free etc
}

static void	last_cmd(t_script script, char **path_env, int *pipein, int pid2)
{
	int	i;
	int ret;

	i = script.cmd_count - 1;
	ret = check_builtin(script.commands[i].cmd);
	if (pid2 == 0 && !ret)
		last_child(script, path_env, pipein, i);
	else if (pid2 && ret)
	{
		handle_builtin(ret, script, 0);
		close(pipein[0]);
		close(pipein[1]);
	}
	wait(0);
	//end of function free everything etc
	//free(path_env);
}

int	pipex(t_script script, char **path_env)
{
	int	pipe1[2];
	int	pipe2[2];
	int	check;
	int	pid1;
	int	pid2;
	int ret;

	check = 0;
	if (pipe(pipe1) == -1)
		return (1); //error

	pid1 = fork();
	if (pid1 == -1)
		return (1); //error
	ret = check_builtin(script.commands[0].cmd);
	if (pid1 == 0 && !ret)
		first_child(script, path_env, pipe1);
	else if (pid1 && ret)
	{
		if (dup2(pipe1[1], STDOUT_FILENO) == -1)
			exit(1);
		handle_builtin(ret, script, 0);
		close(pipe1[0]);
	}
	wait(0);
	close(pipe1[1]);

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