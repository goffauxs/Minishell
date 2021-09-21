#include "minishell.h"

int	builtin_echo(t_command command)
{
	int		i;
	size_t	flag;

	i = 0;
	flag = 0;
	if (!command.argv[1])
		return (0);
	i++;
	if (command.argv[i][flag] == '-')
	{
		flag++;
		while (command.argv[i][flag] == 'n' || command.argv[i][flag] == 'e' || command.argv[i][flag] == 'E')
			flag++;
		if (flag == ft_strlen(command.argv[i]))
			i++;
		else
			flag = 0;
	}
	while (command.argv[i])
	{
		write(1, command.argv[i], ft_strlen(command.argv[i]));
		if (i != command.argc - 1)
			write(1, " ", 1);
		i++;
	}
	if (flag == 0)
		write(1, "\n", 1);
	return (0);
}

int	builtin_cd(t_command command)
{
	char	*home;

	home = getenv("HOME");
	if (!command.argv[1])
		return (chdir(home));
	return (chdir(command.argv[1]));
}

int builtin_exit(t_command command)
{
	int j;
	if(command.argv[1])
	{
		j = 0;
		while(command.argv[1][j])
		{
			if(command.argv[1][j] == '-')
				j++;
			if(!ft_isdigit(command.argv[1][j]))
			{
				write(1, "exit\n", 5);
				printf("%s: %s: numeric argument required\n",command.argv[0], command.argv[1]);
				rl_on_new_line();
				exit_status = 255;
				return(1);
			}
			j++;
		}
	}
	if(command.argc > 2)
	{
		write(1, "exit\n", 5);
		printf("%s: too many arguments\n", command.argv[0]);
		exit_status = 1;
		rl_on_new_line();
		return(0);
	}
	else
	{
		if(command.argv[1])
		{
			if(ft_strncmp(command.argv[1], "9223372036854775807", 19) != 0)
			{
				if(ft_atoi(command.argv[1]) == -1 && ft_strlen(command.argv[1]) > 18) // attention atoi buggé
				{
					write(1, "exit\n", 5);
					printf("%s: %s: numeric argument required\n",command.argv[0], command.argv[1]);
					rl_on_new_line();
					exit_status = 255;
					return(1);
				}
			}
			exit_status = ft_atoi(command.argv[1]) & 0xFF;
		}
		else
			exit_status = 0;
		rl_on_new_line();
		write(1, "exit\n", 5);
	}
	return(1);
}

int	builtin_pwd(void)
{
	char	*buff;

	buff = (char *)malloc(sizeof(char) * MAX_PATH_LEN);
	if (!buff)
		return (1);
	buff = getcwd(buff, MAX_PATH_LEN);
	if (!buff)
		return (1);
	write(1, buff, ft_strlen(buff));
	write(1, "\n", 1);
	//printf("%s\n", buff);
	free(buff);
	return (0);
}

int	builtin_export(t_script *script) //SEGFAULT
{
	char	**tmp;
	int		i;
	int		end;
	int		cursor;

	if (!script->envp)
		return (1);
	i = 0;
	while (script->envp[i])
		i++;
	tmp = script->envp;
	script->envp = (char **)malloc(sizeof(char *) * i + 2);
	end = i;
	i = 0;
	while (tmp[i])
	{
		script->envp[i] = (char *)malloc(sizeof(char) * ft_strlen(tmp[i]) + 1);
		cursor = 0;
		while (tmp[i][cursor])
		{
			script->envp[i][cursor] = tmp[i][cursor];
			cursor++;
		}
		script->envp[i][cursor] = '\0';
		i++;
	}
	script->envp[i++] = NULL; //replace with exported var
	script->envp[i] = NULL;
	// printf("%s\n", script->envp[i]);
	return (0);
}

int	builtin_env(char **envp)
{
	int	i;

	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		write(1, envp[i], ft_strlen(envp[i]));
		write(1, "\n", 1);
		i++;
	}
	//printf("%s\n", envp[i++]);
	return (0);
}
