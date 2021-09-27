/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 17:17:51 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/27 14:51:23 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	free_tab(int i, char **tmp)
{
	while (--i >= 0)
		free(tmp[i]);
	free(tmp);
}

int	strdup_iteration(char **array1, char **array2)
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
	while (array[i])
		i++;
	return (i);
}
