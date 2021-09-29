/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 14:55:52 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/29 10:06:41 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	flag_handling(t_command command, size_t *flag, int *i)
{
	(*flag)++;
	while (command.argv[*i][*flag] == 'n' || command.argv[*i][*flag] == 'e'
		|| command.argv[*i][*flag] == 'E')
		(*flag)++;
	if (*flag == ft_strlen(command.argv[*i]))
		(*i)++;
	else
		*flag = 0;
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
	size_t	flag;

	i = 0;
	flag = 0;
	if (!command.argv[1])
	{
		write(1, "\n", 1);
		return (0);
	}
	i++;
	if (command.argv[i][flag] == '-' && command.argv[i][flag + 1] == 'n')
		flag_handling(command, &flag, &i);
	while (command.argv[i])
		i = echo_print(command, i);
	if (flag == 0)
		write(1, "\n", 1);
	return (0);
}
