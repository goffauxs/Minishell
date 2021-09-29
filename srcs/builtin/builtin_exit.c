/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:00 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/29 10:04:37 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exit_numeric_arg(t_command command)
{
	write(1, "exit\n", 5);
	printf("%s: %s: numeric argument required\n",
		command.argv[0], command.argv[1]);
	rl_on_new_line();
	g_exit_status = 255;
	return (1);
}

static int	exit_too_many_arg(t_command command)
{
	write(1, "exit\n", 5);
	printf("%s: too many arguments\n", command.argv[0]);
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
			if (ft_strncmp(command.argv[1], "9223372036854775807", 19) != 0)
			{
				if (ft_atoi(command.argv[1]) == -1
					&& ft_strlen(command.argv[1]) > 18)
					return (exit_numeric_arg(command));
			}
			g_exit_status = ft_atoi(command.argv[1]) & 0xFF;
		}
		else
			g_exit_status = 0;
		rl_on_new_line();
		write(1, "exit\n", 5);
	}
	return (1);
}
