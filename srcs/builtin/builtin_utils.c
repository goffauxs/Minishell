/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:17:51 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/30 18:29:18 by rvan-aud         ###   ########.fr       */
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
