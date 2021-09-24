/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:55:52 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/24 15:01:30 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_print(t_command command, int i)
{
	write(1, command.argv[i], ft_strlen(command.argv[i]));
	if (i != command.argc - 1)
		write(1, " ", 1);
	i++;
	return (i);
}

int	builtin_echo(t_command command)
{
	int		i;
	size_t	flag;

	i = 0;
	flag = 0;
	if (!command.argv[1])
		return (0);
	i++;
	if (command.argv[i][flag] == '-')
	{
		flag++;
		while (command.argv[i][flag] == 'n' || command.argv[i][flag] == 'e'
			|| command.argv[i][flag] == 'E')
			flag++;
		if (flag == ft_strlen(command.argv[i]))
			i++;
		else
			flag = 0;
	}
	while (command.argv[i])
		i = echo_print(command, i);
	if (flag == 0)
		write(1, "\n", 1);
	return (0);
}
