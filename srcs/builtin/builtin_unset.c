/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:08 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/27 19:31:58 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	loopunset(t_script *script, char *arg, int len, int loop)
{
	int		i;
	int		j;
	char	*str;
	char	**tmp;

	tmp = malloc(sizeof(char *) * len);
	i = 0;
	j = 0;
	str = ft_strjoin(arg, "=");
	while (script->envp[i])
	{
		if (ft_strncmp(script->envp[i], str, ft_strlen(str)))
		{
			tmp[j] = script->envp[i];
			j++;
		}
		// else if (loop)
		// {
		// 	free(script->envp[i]);
		// }
		i++;
	}
	free(str);
	tmp[j] = NULL;
	if (loop)
		free(script->envp);
	script->envp = tmp;
	return (1);
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
	int		var;
	int		loop;

	if (!script->envp)
		return (1);
	var = 1;
	loop = 0;
	while (command.argv[var])
	{
		if (!(check_exisiting(script, command, var))
			|| !(check_invalid(command, var)))
		{
			var++;
			continue ;
		}
		loopunset(script, command.argv[var], env_len(script->envp), loop);
		loop++;
		var++;
	}
	return (0);
}
