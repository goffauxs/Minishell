/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/24 15:48:00 by sgoffaux         ###   ########.fr       */
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
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		g_pid = 0;
		ret = parse(&script, &line_buf);
		if (ret == 1)
			continue ;
		else if (ret == 2)
		{
			rl_replace_line("exit", 0);
			rl_redisplay();
			break ;
		}
		if (script.cmd_count > 0)
		{
			script.termios_p.c_lflag |= ECHOCTL;
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &script.termios_p);
			if (handle_cmd(&script))
				break ;
			script.termios_p.c_lflag &= ~ECHOCTL;
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &script.termios_p);
		}
		free_commands(&script);
	}
	script.termios_p.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &script.termios_p);
}
