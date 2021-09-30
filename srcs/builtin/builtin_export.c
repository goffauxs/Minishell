/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:55:39 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/30 15:33:42 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_exisiting_export(char **envp, char *str)
{
	int		i;
	char	*tmp;
	char	*bis;

	i = 0;
	tmp = ft_strdup(str);
	bis = ft_strnstr(tmp, "+=", ft_strlen(tmp));
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
	if (bis)
	{
		while (envp[i])
		{
			if (!ft_strncmp(tmp, envp[i], ft_strlen(tmp)))
			{
				free(tmp);
				return (i);
			}
			i++;
		}
	}
	free(tmp);
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
	i = copy_env(script->envp, tmp);
	if (exist)
	{
		if (ft_strnstr(argv[var], "+=", ft_strlen(argv[var])))
			tmp[ch] = ft_strjoin_free(tmp[ch], ft_strdup(ft_strchr(argv[var], '=') + 1));
		else
		{
			free(tmp[ch]);
			tmp[ch] = ft_strdup(argv[var]);
		}
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

int    builtin_export(t_script *script, t_command command)
{
    int        var;
    int        err;

    if (!script->envp)
        return (1);
    var = 1;
    err = 1;
    while (command.argv[var])
    {
        if (ft_isdigit(command.argv[var][0]) || !(ft_strncmp(command.argv[var], "=", 1))
            || !ft_strchr(command.argv[var], '='))
        {
            if (ft_isdigit(command.argv[var][0]) || !(ft_strncmp(command.argv[var], "=", 1)))
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
