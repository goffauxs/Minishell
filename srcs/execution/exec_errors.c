/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:28:12 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/09/29 10:00:00 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fork_error(char **path_env)
{
	write(2, "Error: fork failed\n", 19);
	g_exit_status = 1;
	free_path_env(path_env);
}

void	pipe_error(char **path_env)
{
	write(2, "Error: pipe failed\n", 19);
	g_exit_status = 1;
	free_path_env(path_env);
}
