#include "minishell.h"

static void	first_cmd(t_script script, char **path_env, int *pipe1)
{
	//if (!script.commands.in)
	if (dup2(pipe1[1], STDOUT_FILENO) == -1)
	{
		//free tout ce qu'il faut + exec_status = 1 ou 126
		exit(1);
	}
	close pipe1[0];
	exec_cmd(path_env, script.commands[0].argv, script.envp);
	write(2, "command not found\n", 18);
	//free etc
}

static int	other_cmds(t_script script, char **path_env, int *in, int *out)
{

}

static int	loop(t_script script, char **path_env, int *pipe1)
{
	int	i;
	int	check;
	int	pipe2[2];

	i = 1;
	while (i < script.cmd_count)
	{
		if (check == 0)
		{
			if (pipe(pipe2) == -1)
				return (1) //error
			other_cmds(script, path_env, pipe1, pipe2);
			check = 1;
		}
		else if (check == 1)
		{
			if (pipe(pipe1) == -1)
				return (1) //error
			other_cmds(script, path_env, pipe2, pipe1);
			check = 0;
		}
		i++;
	}
}

int	pipex(t_script script, char **path_env)
{
	int	pipe1[2];
	int	check;
	int	pid1;
	int	pid2;

	if (pipe(pipe1) == -1)
		return (1); //error
	pid1 = fork();
	if (pid1 == -1)
		return (1); //error
	if (pid1 == 0)
		first_cmd();
	loop(script, path_env, pipe1);
	wait(0);
}
