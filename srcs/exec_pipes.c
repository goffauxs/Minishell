#include "minishell.h"

static void	middle_child(t_script script, char **path_env, int *pipein, int *pipeout, int i)
{
	// int	fdin;
	// int	fdout;

	// if (script.commands[i].in.name)
	// {
	// 	fdin = open(script.commands[i].in.name, O_RDONLY);
	// 	if (dup2(fdin, STDIN_FILENO) == -1)
	// 	{
	// 		//free tout ce qu'il faut + exec_status = 1 ou 126
	// 		exit(1);
	// 	}
	// }
	// else
	// {
	// 	if (dup2(pipein[0], STDIN_FILENO) == -1)
	// 	{
	// 		//free tout ce qu'il faut + exec_status = 1 ou 126
	// 		exit(1);
	// 	}
	// }
	// if (script.commands[i].out.name)
	// {
	// 	fdout = open(script.commands[i].out.name, script.commands[i].out.flag, 0622);
	// 	if (dup2(fdout, STDOUT_FILENO) == -1)
	// 	{
	// 		//free tout ce qu'il faut + exec_status = 1 ou 126
	// 		exit(1);
	// 	}
	// }
	// else
	// {
	// 	if (dup2(pipeout[1], STDOUT_FILENO) == -1)
	// 	{
	// 		//free tout ce qu'il faut + exec_status = 1 ou 126
	// 		exit(1);
	// 	}
	// }
	if (dup2(pipein[0], STDIN_FILENO) == -1)
	{
		write(2, "dup2 error\n", 11);
		//free tout ce qu'il faut + exec_status = 1 ou 126
		exit(1);
	}
	if (dup2(pipeout[1], STDOUT_FILENO) == -1)
	{
		write(2, "dup2 error\n", 11);
		//free tout ce qu'il faut + exec_status = 1 ou 126
		exit(1);
	}
	close(pipein[1]);
	close(pipeout[0]);
	exec_cmd(path_env, script.commands[i].argv, script.envp);
	write(2, "command not found\n", 18);
	//free etc
}

static int	middle_cmds(t_script script, char **path_env, int *pipein, int *pipeout, int i)
{
	int	pid;
	int	ret;
	int	saved_in;
	int	saved_out;

	ret = check_builtin(script.commands[i].cmd);
	if (!ret)
	{
		pid = fork();
		if (pid == -1)
			return (1); //error
		if (pid == 0)
			middle_child(script, path_env, pipein, pipeout, i);
		close(pipein[0]);
		close(pipeout[1]);
		wait(0);
	}
	else
	{
		if (ret != 7)
		{
			saved_in = dup(0);
			saved_out = dup(1);
			if (dup2(pipein[0], STDIN_FILENO) == -1)
				exit(1); //error
			if (dup2(pipeout[1], STDOUT_FILENO) == -1)
				exit(1); //error
			handle_builtin(ret, script, i);
			close(pipein[0]);
			close(pipeout[1]);
			dup2(saved_in, STDIN_FILENO);
			dup2(saved_out, STDOUT_FILENO);
			close(saved_in);
			close(saved_out);
		}
	}
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
	// char	buff[80];
	// read(pipe2[0], buff, 80);
	// write(2, &buff, 80);
	return (check);
}

static void	first_child(t_script script, char **path_env, int *pipe1)
{
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

static void	last_child(t_script script, char **path_env, int *pipein, int i)
{
	//if (!script.commands[i].out)
	//	|
	//	v

	// char	buff[80];
	// read(pipein[0], buff, 80);
	// write(2, &buff, 80);
	if (dup2(pipein[0], STDIN_FILENO) == -1)
	{
		write(2, "dup2 error\n", 11);
		//free tout ce qu'il faut + exec_status = 1 ou 126
		exit(1);
	}
	close(pipein[1]);
	close(pipein[0]);
	exec_cmd(path_env, script.commands[i].argv, script.envp);
	write(2, "command not found\n", 18);
	//free etc
}

static void	last_cmd(t_script script, char **path_env, int *pipein, int pid2)
{
	int	i;

	i = script.cmd_count - 1;
	if (pid2 == 0)
		last_child(script, path_env, pipein, i);
	// close(pipein[0]);
	// close(pipein[1]);
	//end of function free everything etc
	// free(path_env);
}

int	pipex(t_script script, char **path_env)
{
	int	pipe1[2];
	int	pipe2[2];
	int	check;
	int	pid1;
	int	pid2;
	int	ret;
	int	saved_stdout;

	check = 0;
	if (pipe(pipe1) == -1)
		return (1); //error
	ret = check_builtin(script.commands[0].cmd);
	if (!ret)
	{
		pid1 = fork();
		if (pid1 == -1)
			return (1); //error
		if (pid1 == 0)
			first_child(script, path_env, pipe1);
		close(pipe1[1]);
		wait(0);
	}
	else
	{
		if (ret != 7)
		{
			saved_stdout = dup(1);
			if (dup2(pipe1[1], STDOUT_FILENO) == -1)
				exit(1); //error
			handle_builtin(ret, script, 0);
			close(pipe1[1]);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdout);
		}
	}
	check = middle_loop(script, path_env, pipe1, pipe2);
	if (check == -1)
		return (1); //error
	// char	buff[80];
	// read(pipe1[0], buff, 80);
	// write(2, &buff, 80);
	ret = check_builtin(script.commands[script.cmd_count - 1].cmd);
	if (!ret)
	{
		pid2 = fork();
		if (pid2 == -1)
			return (1); //error
		if (check == 1)
			last_cmd(script, path_env, pipe2, pid2);
		else if (check == 0)
			last_cmd(script, path_env, pipe1, pid2);
		wait(0);
	}
	else
	{
		if (ret != 7)
		{
			if (check == 1)
			{
				if (dup2(pipe2[0], STDIN_FILENO) == -1)
					exit(1); //error
			}
			else if (check == 0)
			{
				if (dup2(pipe1[0], STDIN_FILENO) == -1)
					exit(1); //error
			}
			handle_builtin(ret, script, 0);
		}
	}
	return (0);
}