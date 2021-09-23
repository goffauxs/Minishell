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

int builtin_exit(t_command command, t_script *script)
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
				script->exit_status = 255;
				return(1);
			}
			j++;
		}
	}
	if(command.argc > 2)
	{
		write(1, "exit\n", 5);
		printf("%s: too many arguments\n", command.argv[0]);
		script->exit_status = 1;
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
					script->exit_status = 255;
					return(1);
				}
			}
			script->exit_status = ft_atoi(command.argv[1]) & 0xFF;
		}
		else
			script->exit_status = 0;
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
	free(buff);
	return (0);
}

int		check_existing(char **envp, char *str)
{
	int		j;
	int		i;

	j = 0;
	i = 0;
	while(str[j] && str[j] != '=')
		j++;
	while(envp[i])
	{
		if(!ft_strncmp(str, envp[i], j))
			return(i);
		i++;
	}
	return (0);
}

int		has_char(char *str, char c)
{
	int i;
	i = 0;
	while(str[i])
	{
		if(str[i] == c)
			return(1);
		i++;
	}
	return(0);
}

int	builtin_export(char ***envp, t_command command)
{
	char	**tmp;
	int		i;
	int		j;
	int		exist;

	if (!(*envp))
		return (1);
	i = 1;
	while (command.argv[i])
	{
		exist = 0;
		if (ft_isdigit(command.argv[i][0]))
		{
			printf("export: '%s': not a valid identifier\n", command.argv[i]);
			i++;
			continue;
		}
		if (!has_char(command.argv[i], '='))
		{
			i++;
			continue;
		}
		if (check_existing((*envp), command.argv[i]))
			exist = 1;
		j = 0;
		while ((*envp)[j])
			j++;
		tmp = malloc(sizeof(char *) * (j + 2 - exist));
		j = 0;
		while ((*envp)[j])
		{
			tmp[j] = ft_strdup((*envp)[j]);
			j++;
		}
		if (exist)
		{
			free(tmp[check_existing((*envp), command.argv[i])]);
			tmp[check_existing((*envp), command.argv[i])] = ft_strdup(command.argv[i]);
			tmp[j] = NULL;
		}
		else
		{
			tmp[j] = ft_strdup(command.argv[i]);
			tmp[j + 1] = NULL;
		}
		j = 0;
		while(tmp[j])
		{
			(*envp)[j] = ft_strdup(tmp[j]);
			j++;
		}
		while(--j >= 0)
			free(tmp[j]);
		free(tmp);
		i++;
	}
	// int count = 0;
    // while((*envp)[count])
    // {
    //  printf(" env = |%s|\n", (*envp)[count]);
    //  count++;
    // }
	return(0);
}
//

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
	return (0);
}
