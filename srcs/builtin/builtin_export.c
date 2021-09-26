/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:55:39 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/26 16:11:10 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ch -> check_exisiting_export
int	ch(char **envp, char *str)
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

t_script	*loopexport(t_script *script, t_command command, int var)
{
	int		exist;
	int		i;
	char	**tmp;

	exist = 0;
	if (ch(script->envp, command.argv[var]))
		exist = 1;
	i = env_len(script->envp);
	tmp = malloc(sizeof(char *) * (i + 2 - exist));
	i = strdup_iteration(script->envp, tmp);
	if (exist)
	{
		free(tmp[ch(script->envp, command.argv[var])]);
		tmp[ch(script->envp, command.argv[var])] = ft_strdup(command.argv[var]);
		tmp[i] = NULL;
	}
	else
	{
		tmp[i] = ft_strdup(command.argv[var]);
		tmp[i + 1] = NULL;
	}
	i = strdup_iteration(tmp, script->envp);
	script->envp[i] = NULL;
	free_tab(i, tmp);
	return (script);
}

int	builtin_export(t_script *script, t_command command)
{
	int		var;

	if (!script->envp)
		return (1);
	var = 1;
	while (command.argv[var])
	{
		if (ft_isdigit(command.argv[var][0]))
		{
			printf("export: '%s': not a valid identifier\n",
				command.argv[var]);
			var++;
			continue ;
		}
		if (!has_char(command.argv[var], '='))
		{
			var++;
			continue ;
		}
		script = loopexport(script, command, var);
		var++;
	}
	return (0);
}
