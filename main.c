/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/27 17:09:32 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

static void	termios(t_script *script)
{
	tcgetattr(STDIN_FILENO, &script->termios_p);
	script->termios_p.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &script->termios_p);
}

static void	main_loop(t_script *script, char **line_buf)
{
	int	ret;

	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		g_pid = 0;
		ret = parse(script, line_buf);
		if (ret == 1)
			continue ;
		else if (ret == 2)
			break ;
		if (script->cmd_count > 0)
		{
			if (handle_cmd(script))
				break ;
		}
		free_commands(script);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_script	script;
	char		*line_buf;

	(void)argc;
	(void)argv;
	script.envp = envp;
	line_buf = NULL;
	signal(SIGINT, sig_handler);
	termios(&script);
	main_loop(&script, &line_buf);
	exit(script.exit_status);
}
