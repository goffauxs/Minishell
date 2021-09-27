/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_pwd_env.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:14 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/24 14:56:17 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_command command)
{
	char	*home;

	home = getenv("HOME");
	if (!command.argv[1])
		return (chdir(home));
	return (chdir(command.argv[1]));
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
