/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:00 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 14:23:28 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exit_numeric_arg(t_command command)
{
	write(1, "exit\n", 5);
	ft_putstr_fd("exit : ", 2);
	ft_putstr_fd(command.argv[1], 2);
	ft_putendl_fd(": numeric argument required", 2);
	rl_on_new_line();
	g_exit_status = 2;
	return (1);
}

static int	exit_too_many_arg(t_command command)
{
	(void)command;
	write(1, "exit\n", 5);
	ft_putendl_fd("exit: too many arguments", 2);
	g_exit_status = 1;
	rl_on_new_line();
	return (0);
}

static int	check_flag(t_command command)
{
	int	j;

	j = 0;
	while (command.argv[1][j])
	{
		if (command.argv[1][j] == '-')
			j++;
		if (!ft_isdigit(command.argv[1][j]))
			return (exit_numeric_arg(command));
		j++;
	}
	return (0);
}

int	builtin_exit(t_command command)
{
	if (command.argv[1])
	{
		if (check_flag(command))
			return (1);
	}
	if (command.argc > 2)
		return (exit_too_many_arg(command));
	else
	{
		if (command.argv[1])
		{
			if ((ft_atoi(command.argv[1]) == -1
					&& ft_strncmp(command.argv[1], "-1", 2))
				|| (ft_atoi(command.argv[1]) == 0
					&& ft_strncmp(command.argv[1], "0", 1)))
				return (exit_numeric_arg(command));
			g_exit_status = ft_atoi(command.argv[1]) & 0xFF;
		}
		else
			g_exit_status = 0;
		rl_on_new_line();
		write(1, "exit\n", 5);
	}
	return (1);
}
