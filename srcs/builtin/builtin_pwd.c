/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:50:02 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/30 11:57:25 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_command command, char **envp)
{
	char	*buff;

	if (command.argv[1])
	{
		printf("pwd: too many arguments\n");
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
