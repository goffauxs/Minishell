/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:08 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/24 14:56:11 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_script *script, t_command command)
{
	char	**tmp;
	int		var;
	int		i;
	int		j;
	int		check;

	if (!script->envp)
		return (1);
	var = 1;
	while (command.argv[var])
	{
		if (ft_isdigit(command.argv[var][0]) || has_char(command.argv[var], '='))
		{
			printf("unset: '%s': not a valid identifier\n", command.argv[var]);
			var++;
			continue ;
		}
		i = 0;
		check = 0;
		while (script->envp[i])
		{
			if (ft_strncmp(script->envp[i], command.argv[var], ft_strlen(command.argv[var])))
				check = 1;
			i++;
		}
		if (!check)
		{
			var++;
			continue ;
		}
		while (script->envp[i])
			i++;
		tmp = malloc(sizeof(char *) * i);
		i = 0;
		j = 0;
		while (script->envp[i])
		{
			if (ft_strncmp(script->envp[i], command.argv[var], ft_strlen(command.argv[var])))
			{
				tmp[j] = ft_strdup(script->envp[i]);
				j++;
			}
			i++;
		}
		tmp[j] = NULL;
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