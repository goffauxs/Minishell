#include "minishell.h"
#include "libft.h"

int	builtin_echo(t_command command)
{
	int		i;
	size_t	flag;

	i = 0;
	flag = 0;
	if (!command.argv[0])
	{
		printf("\n");
		return (0);
	}
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
		printf("%s", command.argv[i]);
		if (command.argv[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}

int	builtin_cd(t_command command)
{
	int		i;
	char	*home;

	i = 0;
	home = getenv("HOME");
	if (!command.argv[0])
		return (chdir(home));
	i++;
	return (chdir(command.argv[i]));
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
				printf("%s: %s: numeric argument required\n",command.cmd, command.argv[1]);
				rl_on_new_line();
				exit_status = 255;
				return(0);
			}
			j++;
		}
	}
	if(command.argc > 2)
	{
		write(1, "exit\n", 5);
		printf("%s: too many arguments\n", command.cmd);
		rl_on_new_line();
		return(1);
	}
	else
	{
		if(command.argv[1])
		{
			exit_status = ft_atoi(command.argv[1]) % 256;
			if(exit_status < 0)
				exit_status = exit_status * -1;
		}
		else	
			exit_status = 0;
		rl_on_new_line();
		write(1, "exit\n", 5);
	}
	return(0);
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
	printf("%s\n", buff);
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
		printf("%s\n", envp[i++]);
	return (0);
}
