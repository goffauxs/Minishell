/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:14 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/30 17:39:44 by rvan-aud         ###   ########.fr       */
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

static void	replace_env(char *str, char *name, char ***envp)
{
	int		idx;

	idx = get_env(name, *envp);
	free((*envp)[idx]);
	(*envp)[idx] = ft_strjoin_free(ft_strdup(name), str);
}

static int	change_dir(char *path, char ***envp)
{
	int		ret;
	char	*pwd;

	replace_env(get_env_content("PWD", *envp), "OLDPWD=", envp);
	ret = chdir(path);
	if (ret == -1)
	{
		printf("cd: %s: no such file or directory\n", path);
		return (1);
	}
	pwd = NULL;
	pwd = getcwd(pwd, MAX_PATH_LEN);
	replace_env(pwd, "PWD=", envp);
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
		printf("cd: too many arguments\n");
		return (1);
	}
	return (change_dir(command.argv[1], &envp));
}
