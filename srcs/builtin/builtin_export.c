/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:55:39 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/24 14:57:06 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_existing_export(char **envp, char *str)
{
	int		j;
	int		i;

	j = 0;
	i = 0;
	while (str[j] && str[j] != '=')
		j++;
	while (envp[i])
	{
		if (!ft_strncmp(str, envp[i], j))
			return (i);
		i++;
	}
	return (0);
}

int	has_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
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
			continue ;
		}
		if (!has_char(command.argv[var], '='))
		{
			var++;
			continue ;
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
		while (tmp[i])
		{
			script->envp[i] = ft_strdup(tmp[i]);
			i++;
		}
		script->envp[i] = NULL;
		while (--i >= 0)
			free(tmp[i]);
		free(tmp);
		var++;
	}
	return (0);
}
