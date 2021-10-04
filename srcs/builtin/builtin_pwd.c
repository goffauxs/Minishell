/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:50:02 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/04 14:26:44 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_command command, char **envp)
{
	char	*buff;

	if (command.argv[1])
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (1);
	}
	buff = NULL;
	buff = getcwd(buff, MAX_PATH_LEN);
	if (!buff)
	{
		free(buff);
		buff = get_env_content("PWD", envp);
		ft_putendl_fd(buff, 1);
	}
	else
		ft_putendl_fd(buff, 1);
	free(buff);
	return (0);
}
