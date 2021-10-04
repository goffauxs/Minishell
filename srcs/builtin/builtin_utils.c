/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:17:51 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 13:50:59 by mdeclerf         ###   ########.fr       */
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
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-' || str[i] == '.' || str[i] == '{' || str[i] == '}'
			|| str[i] == '*' || str[i] == '#' || str[i] == '@' || str[i] == '!'
			|| str[i] == '^' || str[i] == '~')
			return (0);
		if (str[i] == '+' && str[i + 1] != '=')
			return (0);
		i++;
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
			printf("declare -x %s\n", str[i]);
			i++;
		}
	}
	else if (cmd.argv[1][0] == '\0')
	{
		printf("Syntax error\n");
		return (1);
	}
	return (0);
}
