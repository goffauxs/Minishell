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
	free(buff);
	return (0);
}

///
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
			// printf("tmp[j] %s\n", tmp[j]);
			j++;
		}
		if (exist)
		{
			free(tmp[check_existing((*envp), command.argv[i])]);
			tmp[check_existing((*envp), command.argv[i])] = ft_strdup(command.argv[i]);
			// printf("tmp[check_exisiting((*envp), command.argv[i])] %s\n", tmp[check_existing((*envp), command.argv[i])]);
			// printf("ft_strdup(command.argv[i]) %s\n", ft_strdup(command.argv[i]));
		}
		else
		{
			tmp[j] = ft_strdup(command.argv[i]);
			// printf("NEW ARG tmp[j] %s\n", tmp[j]);
		}
		j++;
		tmp[j] = NULL;
		j = 0;
		while(tmp[j])
		{
			printf("tmp[j] %s\n", tmp[j]);
			j++;
		}
		*envp = tmp;
		// j = 0;
		// while(tmp[j])
		// {
		// 	free(tmp[j]);
		// 	j++;
		// }
		// free(tmp);
		i++;
	}
	
	// int count = 0;
	// while((*envp)[count])
	// {
	// 	printf(" env = |%s|\n", (*envp)[count]);
	// 	count++;
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


// EXPORT BUG
// int		check_exisiting(char **envp, char *str)
// {
// 	int		j;
// 	int		i;

// 	j = 0;
// 	i = 0;
// 	while(str[j] && str[j] != '=')
// 		j++;
// 	while(envp[i])
// 	{
// 		if(!ft_strncmp(str, envp[i], j))
// 			return(i);
// 		i++;
// 	}
// 	return (0);
// }

// int		has_char(char *str, char c)
// {
// 	int i;
// 	i = 0;
// 	while(str[i])
// 	{
// 		if(str[i] == c)
// 			return(0);
// 		i++;
// 	}
// 	return(1);
// }

// int	builtin_export(char ***envp, t_command command)
// {
// 	char	**tmp;
// 	int		i;
// 	int		j;

// 	if (!(*envp))
// 		return (1);
// 	i = 1;
// 	j = 0;
// 	while (command.argv[i])
// 	{
// 		if (ft_isdigit(command.argv[i][0]))
// 		{
// 			printf("export: '%s': not a valid identifier\n", command.argv[i]);
// 			j++;
// 		}
// 		if (!has_char(command.argv[i], '='))
// 			j++;
// 		if (check_exisiting((*envp), command.argv[i]))
// 			j++;
// 		i++;
// 	}
// 	i = 0;
// 	while ((*envp)[i])
// 		i++;
// 	tmp = malloc(sizeof(char *) * (i + command.argc - j));
// 	//
	
// 	i = 0;
// 	j = 1;
// 	while ((*envp)[i])
// 	{
// 		tmp[i] = ft_strdup((*envp)[i]);
// 		i++;
// 	}
// 	while(command.argv[j])
// 	{
// 		if(check_exisiting((*envp), command.argv[j]) && !ft_isdigit(command.argv[j][0]) && has_char(command.argv[j], '='))
// 		{
// 			free(tmp[check_exisiting((*envp), command.argv[j])]);
// 			tmp[check_exisiting((*envp), command.argv[j])] = ft_strdup(command.argv[j]);
// 		}
// 		j++;
// 	}
// 	j = 1;
// 	while (command.argv[j])
// 	{
// 		if (check_exisiting((*envp), command.argv[j]) || ft_isdigit(command.argv[j][0]) || !has_char(command.argv[j], '='))
// 			j++;
// 		else if (!ft_isdigit(command.argv[j][0]) && has_char(command.argv[j], '='))
// 		{
// 			tmp[i] = ft_strdup(command.argv[j]);
// 			j++;
// 			i++;
// 		}
// 	}
// 	tmp[i] = NULL;
// 	*envp = tmp;

// 	i = 0;
// 	while((*envp)[i])
// 	{
// 		printf(" env = |%s|\n", (*envp)[i]);
// 		i++;
// 	}
// 	return (0);
// }
// //