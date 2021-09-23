/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/23 15:09:39 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_script	script;
	char		*line_buf;

	(void)argc;
	(void)argv;
	script.envp = envp;
	line_buf = NULL;
	while (1)
	{	
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
		if (parse(&script, &line_buf))
			continue ;
		if (script.cmd_count > 0)
			handle_cmd(&script);
		free_commands(&script);
		if (!ft_strncmp(line_buf, "exit", 4) && script.cmd_count == 1)
		{
			free(line_buf);
			break ;
		}
	}
}
