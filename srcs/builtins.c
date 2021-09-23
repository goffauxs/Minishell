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

int		check_existing_export(char **envp, char *str)
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

int	builtin_export(t_script *script, t_command command)
{
	char	**tmp;
	int		var;
	int		i;
	int		exist;

	if (!script->envp)
		return (1);
	var = 1;
	while (command.argv[var])
	{
		exist = 0;
		if (ft_isdigit(command.argv[var][0]))
		{
			printf("export: '%s': not a valid identifier\n", command.argv[var]);
			var++;
			continue;
		}
		if (!has_char(command.argv[var], '='))
		{
			var++;
			continue;
		}
		if (check_existing_export(script->envp, command.argv[var]))
			exist = 1;
		i = 0;
		while (script->envp[i])
			i++;
		tmp = malloc(sizeof(char *) * (i + 2 - exist));
		i = 0;
		while (script->envp[i])
		{
			tmp[i] = ft_strdup(script->envp[i]);
			i++;
		}
		if (exist)
		{
			free(tmp[check_existing_export(script->envp, command.argv[var])]);
			tmp[check_existing_export(script->envp, command.argv[var])] = ft_strdup(command.argv[var]);
			tmp[i] = NULL;
		}
		else
		{
			tmp[i] = ft_strdup(command.argv[var]);
			tmp[i + 1] = NULL;
		}
		i = 0;
		while(tmp[i])
		{
			script->envp[i] = ft_strdup(tmp[i]);
			i++;
		}
		script->envp[i] = NULL;
		while(--i >= 0)
			free(tmp[i]);
		free(tmp);
		var++;
	}
	return(0);
}
//

int	builtin_unset(t_script *script, t_command command)
{
	char	**tmp;
	int		var;
	int		i;
	int		j;
	int		check;

	if (!script->envp)
		return (1);
	var = 1;
	while (command.argv[var])
	{
		if (ft_isdigit(command.argv[var][0]) || has_char(command.argv[var], '='))
		{
			printf("unset: '%s': not a valid identifier\n", command.argv[var]);
			var++;
			continue;
		}
		i = 0;
		check = 0;
		while (script->envp[i])
		{
			if (ft_strncmp(script->envp[i], command.argv[var], ft_strlen(command.argv[var])))
				check = 1;
			i++;
		}
		if (!check)
		{
			var++;
			continue;
		}
		while (script->envp[i])
			i++;
		tmp = malloc(sizeof(char *) * i);
		i = 0;
		j = 0;
		while (script->envp[i])
		{
			if (ft_strncmp(script->envp[i], command.argv[var], ft_strlen(command.argv[var])))
			{
				tmp[j] = ft_strdup(script->envp[i]);
				j++;
			}
			i++;
		}
		tmp[j] = NULL;
		i = 0;
		while(tmp[i])
		{
			script->envp[i] = ft_strdup(tmp[i]);
			i++;
		}
		script->envp[i] = NULL;
		while(--i >= 0)
			free(tmp[i]);
		free(tmp);
		var++;
	}
	return(0);
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
	return (0);
}