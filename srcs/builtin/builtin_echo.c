/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:55:52 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/04 13:48:50 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	flag_handling(t_command command, int *flag, int *i)
{
	size_t	j;

	j = 2;
	while (command.argv[*i][j] == 'n' || command.argv[*i][j] == 'e'
		|| command.argv[*i][j] == 'E')
		j++;
	if (j == ft_strlen(command.argv[*i]))
	{
		(*i)++;
		(*flag)++;
	}
	else
		*flag = 0;
	return (*flag);
}

static int	echo_print(t_command command, int i)
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
	int		flag;

	i = 1;
	flag = 0;
	if (!command.argv[1])
	{
		write(1, "\n", 1);
		return (0);
	}
	while (command.argv[i] && command.argv[i][0] == '-'
		&& command.argv[i][1] == 'n')
	{
		if (!flag_handling(command, &flag, &i))
			break ;
	}
	if (!command.argv[i])
		return (0);
	while (command.argv[i])
		i = echo_print(command, i);
	if (flag == 0)
		write(1, "\n", 1);
	return (0);
}
