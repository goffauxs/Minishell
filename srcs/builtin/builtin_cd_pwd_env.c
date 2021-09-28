/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_pwd_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:14 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/28 15:50:05 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_command command)
{
	char	*home;
	int		ret;

	home = getenv("HOME");
	if (!command.argv[1])
		return (chdir(home));
	if (command.argv[2])
	{
		printf("cd: string not in pwd: %s\n", command.argv[1]);
		return (1);
	}
	ret = chdir(command.argv[1]);
	if (ret == -1)
	{
		printf("cd: no such file or directory: %s\n", command.argv[1]);
		return (1);
	}
	return (ret);
}

int	builtin_pwd(t_command command)
{
	char	*buff;

	if(command.argv[1])
	{
		printf("pwd: too many arguments\n");
		return (1);
	}
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

int	builtin_env(char **envp, t_command command)
{
	int	i;

	if(command.argv[1])
	{
		printf("env: too many arguments\n");
		return (1);
	}
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
