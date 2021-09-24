/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/24 14:52:04 by rvan-aud         ###   ########.fr       */
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
	tcgetattr(STDIN_FILENO, &script.termios_p);
	script.termios_p.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &script.termios_p);
	signal(SIGINT, sig_handler);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		g_pid = 0;
		if (parse(&script, &line_buf))
			continue ;
		if (script.cmd_count > 0)
			handle_cmd(&script);
		free_commands(&script);
		system("leaks minishell");
		if (!ft_strncmp(line_buf, "exit", 4) && script.cmd_count == 1)
			break ;
	}
	return (0);
}
