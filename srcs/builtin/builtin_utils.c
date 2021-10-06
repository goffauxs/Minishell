/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:17:51 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/06 10:39:26 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_env(char **array1, char **array2)
{
	int	i;

	i = 0;
	while (array1[i])
	{
		array2[i] = array1[i];
		i++;
	}
	return (i);
}

int	env_len(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (0);
	while (array[i])
		i++;
	return (i);
}

char	*copy_no_plus(char *argvar)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	tmp = NULL;
	if (ft_strnstr(argvar, "+=", ft_strlen(argvar)))
	{
		tmp = malloc(sizeof(char) * ft_strlen(argvar));
		if (!tmp)
			return (NULL);
		while (argvar[i])
		{
			if (argvar[i] != '+')
				tmp[j++] = argvar[i];
			i++;
		}
		tmp[j] = '\0';
	}
	return (tmp);
}

int	checkvalid(char *str)
{
	const char	*end = ft_strrchr(str, '=');

	while (str && *str && str != end)
	{
		if (*str == '-' || *str == '.' || *str == '{' || *str == '}'
			|| *str == '*' || *str == '#' || *str == '@' || *str == '!'
			|| *str == '^' || *str == '~' || *str == '\"' || *str == '|'
			|| *str == '\'' || *str == '$' || *str == ';' || *str == '&'
			|| !ft_isascii(*str) || ft_isspace(*str))
			return (0);
		if (*str == '+' && *(str + 1) != '=')
			return (0);
		str++;
	}
	return (1);
}

int	export_check(char **str, t_command cmd)
{
	int	i;

	if (!cmd.argv[1])
	{
		i = 0;
		if (!str)
			return (1);
		while (str[i])
		{
			ft_putstr_fd("declare -x ", 2);
			ft_putendl_fd(str[i], 2);
			i++;
		}
	}
	else if (cmd.argv[1][0] == '\0')
	{
		ft_putendl_fd("Minishell: Syntax error", 2);
		return (1);
	}
	return (0);
}
