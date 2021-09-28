/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:55:39 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/28 16:17:41 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_exisiting_export(char **envp, char *str)
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
	i = strdup_iteration(script->envp, tmp);
	if (exist)
	{
		free(tmp[ch]);
		tmp[ch] = ft_strdup(argv[var]);
		tmp[i] = NULL;
	}
	else
	{
		tmp[i] = ft_strdup(argv[var]);
		tmp[i + 1] = NULL;
	}
	free(script->envp);
	script->envp = tmp;
}

int	builtin_export(t_script *script, t_command command)
{
	int		var;
	int		err;

	if (!script->envp)
		return (1);
	var = 1;
	err = 1;
	while (command.argv[var])
	{
		if (ft_isdigit(command.argv[var][0])
			|| !has_char(command.argv[var], '='))
		{
			if (ft_isdigit(command.argv[var][0]))
				printf("export: '%s': not a valid identifier\n",
					command.argv[var]);
			else
				err = 0;
			var++;
			continue ;
		}
		err = 0;
		loopexport(script, command.argv, var, env_len(script->envp));
		var++;
	}
	return (err);
}
