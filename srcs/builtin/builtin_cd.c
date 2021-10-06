/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:14 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/06 15:54:30 by sgoffaux         ###   ########.fr       */
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

	ret = chdir(path);
	if (ret == -1)
	{
		ft_putstr_fd("Minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	replace_env(get_env_content("PWD", *envp), "OLDPWD=", envp);
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
		ft_putendl_fd("Minishell: cd: too many arguments", 2);
		return (1);
	}
	return (change_dir(command.argv[1], &envp));
}
