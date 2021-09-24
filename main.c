/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/24 14:06:36 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_script	script;
	char		*line_buf;
	int			ret;

	(void)argc;
	(void)argv;
	script.envp = envp;
	line_buf = NULL;
	tcgetattr(STDIN_FILENO, &script.termios_p);
	script.termios_p.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &script.termios_p);
	signal(SIGINT, sig_handler);
	while (1)
	{	
		signal(SIGQUIT, SIG_IGN);
		g_pid = 0;
		ret = parse(&script, &line_buf);
		if (ret == 1)
			continue ;
		else if (ret == 2)
			break;
		if (script.cmd_count > 0)
		{
			if(handle_cmd(&script))
			{
				free(line_buf);
				break ;
			}
		}
		free_commands(&script);
	}
}
