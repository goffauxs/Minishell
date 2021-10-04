/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:55:39 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 19:18:17 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	prep_bis(char *tmp, char *bis)
{
	if (bis)
	{
		bis[0] = '=';
		bis[1] = '\0';
	}
	else
	{
		bis = ft_strnstr(tmp, "=", ft_strlen(tmp));
		if (bis)
			bis[0] = '\0';
	}
}

static int	check_exisiting_export(char **envp, char *str)
{
	int		i;
	char	*tmp;
	char	*bis;

	i = 0;
	tmp = ft_strdup(str);
	bis = ft_strnstr(tmp, "+=", ft_strlen(tmp));
	prep_bis(tmp, bis);
	// if (bis)
	// {
	while (envp[i])
	{
		if (!ft_strncmp(tmp, envp[i], ft_strlen(tmp)))
		{
			free(tmp);
			return (i);
		}
		i++;
	}
	// }
	free(tmp);
	return (0);
}

static void	prep_tmp_exist(char *argvar, char **tmp, const int ch, int i)
{
	if (ft_strnstr(argvar, "+=", ft_strlen(argvar)))
		tmp[ch] = ft_strjoin_free(tmp[ch],
				ft_strdup(ft_strchr(argvar, '=') + 1));
	else
	{
		free(tmp[ch]);
		tmp[ch] = ft_strdup(argvar);
	}
	tmp[i] = NULL;
}

static void	loopexport(t_script *script, char **argv, int var, int len)
{
	int			exist;
	int			i;
	char		**tmp;
	const int	ch = check_exisiting_export(script->envp, argv[var]);

	exist = 0;
	if (ch)
		exist = 1;
	tmp = malloc(sizeof(char *) * (len + 2 - exist));
	if (!tmp)
		return ;
	i = copy_env(script->envp, tmp);
	if (exist)
		prep_tmp_exist(argv[var], tmp, ch, i);
	else
	{
		tmp[i] = copy_no_plus(argv[var]);
		if (!tmp[i])
			tmp[i] = ft_strdup(argv[var]);
		tmp[i + 1] = NULL;
	}
	free(script->envp);
	script->envp = tmp;
}

int	builtin_export(t_script *script, t_command cmd)
{
	int	var;
	int	err;

	if (!script->envp || !cmd.argv[1] || cmd.argv[1][0] == '\0')
		return (export_check(script->envp, cmd));
	var = 1;
	err = 1;
	while (cmd.argv[var])
	{
		if (ft_isdigit(cmd.argv[var][0]) || !ft_strchr(cmd.argv[var], '=')
			|| !ft_strncmp(cmd.argv[var], "=", 1) || !checkvalid(cmd.argv[var]))
		{
			if (ft_isdigit(cmd.argv[var][0]) || !checkvalid(cmd.argv[var])
				|| !ft_strncmp(cmd.argv[var], "=", 1))
				error_message_export(cmd.argv[var]);
			else
				err = 0;
			var++;
			continue ;
		}
		err = 0;
		loopexport(script, cmd.argv, var, env_len(script->envp));
		var++;
	}
	return (err);
}
