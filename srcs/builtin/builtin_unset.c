/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:08 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/29 10:07:10 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	loopunset(t_script *script, char *arg, int len)
{
	int		i;
	int		j;
	char	*str;
	char	**tmp;

	tmp = malloc(sizeof(char *) * len);
	if (!tmp)
		return ;
	i = 0;
	j = 0;
	str = ft_strjoin(arg, "=");
	while (script->envp[i])
	{
		if (ft_strncmp(script->envp[i], str, ft_strlen(str)))
			tmp[j++] = script->envp[i];
		else
			free(script->envp[i]);
		i++;
	}
	free(str);
	tmp[j] = NULL;
	free(script->envp);
	script->envp = tmp;
}

static int	check_invalid(t_command command, int var)
{
	if (ft_isdigit(command.argv[var][0])
			|| has_char(command.argv[var], '='))
	{
		printf("unset: '%s': not a valid identifier\n", command.argv[var]);
		return (0);
	}
	return (1);
}

static int	check_exisiting(t_script *script, t_command command, int var)
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
	int		err;

	if (!script->envp)
		return (1);
	var = 1;
	err = 0;
	while (command.argv[var])
	{
		if (!(check_exisiting(script, command, var)))
		{
			var++;
			continue ;
		}
		else if (!(check_invalid(command, var)))
		{
			err = 1;
			var++;
			continue ;
		}
		err = 0;
		loopunset(script, command.argv[var], env_len(script->envp));
		var++;
	}
	return (err);
}
