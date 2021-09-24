/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:08 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/24 17:48:22 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_script	*lop_unset(t_script *script, t_command command, int var, char **tmp)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (script->envp[i])
	{
		if (ft_strncmp(script->envp[i], command.argv[var],
				ft_strlen(command.argv[var])))
		{
			tmp[j] = ft_strdup(script->envp[i]);
			j++;
		}
		i++;
	}
	tmp[j] = NULL;
	i = strdup_iteration(tmp, script->envp);
	script->envp[i] = NULL;
	free_tab(i, tmp);
	return (script);
}

int	check_invalid(t_command command, int var)
{
	if (ft_isdigit(command.argv[var][0])
			|| has_char(command.argv[var], '='))
	{
		printf("unset: '%s': not a valid identifier\n", command.argv[var]);
		return (0);
	}
	return (1);
}

int	check_exisiting(t_script *script, t_command command, int var)
{
	int	check;
	int	i;

	i = 0;
	check = 0;
	while (script->envp[i])
	{
		if (ft_strncmp(script->envp[i], command.argv[var],
				ft_strlen(command.argv[var])))
			check = 1;
		i++;
	}
	return (check);
}

int	builtin_unset(t_script *script, t_command command)
{
	char	**tmp;
	int		var;
	int		i;

	if (!script->envp)
		return (1);
	var = 1;
	while (command.argv[var])
	{
		if (!(check_invalid(command, var)))
		{
			var++;
			continue ;
		}
		if (!(check_exisiting(script, command, var)))
		{
			var++;
			continue ;
		}
		i = env_len(script->envp);
		tmp = malloc(sizeof(char *) * i);
		script = lop_unset(script, command, var, tmp);
		var++;
	}
	return (0);
}
