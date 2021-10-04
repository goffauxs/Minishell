/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:14 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 14:14:53 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_env(char *str, char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(str, envp[i], ft_strlen(str)))
			return (i);
		i++;
	}
	return (-1);
}

static int	replace_env(char *str, char *name, char ***envp)
{
	int		idx;

	idx = get_env(name, *envp);
	if (idx == -1)
	{
		free(str);
		return (1);
	}
	free((*envp)[idx]);
	(*envp)[idx] = ft_strjoin_free(ft_strdup(name), str);
	return (0);
}

static int	change_dir(char *path, char ***envp)
{
	int		ret;
	char	*pwd;

	replace_env(get_env_content("PWD", *envp), "OLDPWD=", envp);
	ret = chdir(path);
	if (ret == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": no such file or directory", 2);
		return (1);
	}
	pwd = NULL;
	pwd = getcwd(pwd, MAX_PATH_LEN);
	ret = replace_env(pwd, "PWD=", envp);
	return (ret);
}

int	builtin_cd(t_command command, char **envp)
{
	char	*home;
	int		ret;

	if (!command.argv[1])
	{
		home = get_env_content("HOME", envp);
		ret = change_dir(home, &envp);
		free(home);
		return (ret);
	}
	if (command.argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	return (change_dir(command.argv[1], &envp));
}
