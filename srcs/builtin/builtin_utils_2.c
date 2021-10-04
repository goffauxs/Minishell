/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:48:16 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 19:39:12 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_message_export(char *message)
{
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(message, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

int	check_space_export(char *str)
{
	int	i;

	i = 0;
	while (str[i++])
	{
		if (ft_isspace(str[i]))
			return (1);
	}
	return (0);
}
