/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:56:00 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/06 13:23:25 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exit_numeric_arg(t_command command, int cmd_count)
{
	if (cmd_count == 1)
		ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("Minishell: exit: ", 2);
	ft_putstr_fd(command.argv[1], 2);
	ft_putendl_fd(": numeric argument required", 2);
	rl_on_new_line();
	g_exit_status = 2;
	return (1);
}

static int	exit_too_many_arg(int cmd_count)
{
	if (cmd_count == 1)
		ft_putstr_fd("exit\n", 2);
	ft_putendl_fd("Minishell: exit: too many arguments", 2);
	g_exit_status = 1;
	rl_on_new_line();
	return (0);
}

int	builtin_exit(t_command command, int cmd_count, char *str)
{
	long long		ret;

	if (str)
		if (!ft_islong(str))
			return (exit_numeric_arg(command, cmd_count));
	if (command.argc > 2)
		return (exit_too_many_arg(cmd_count));
	else
	{
		if (str)
		{
			ret = ft_atol(str);
			if ((ret == -1 && ft_strncmp(str, "-1", 2))
				|| (ret == 0 && !(!ft_strncmp(str, "0", 1)
						|| !ft_strncmp(str, "-0", 2))))
				return (exit_numeric_arg(command, cmd_count));
			g_exit_status = (int)(ft_atol(str) & 0xFF);
		}
		if (cmd_count == 1)
			write(1, "exit\n", 5);
	}
	return (1);
}
