/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/27 10:45:17 by mdeclerf         ###   ########.fr       */
=======
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/27 14:37:38 by sgoffaux         ###   ########.fr       */
>>>>>>> origin/main
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

<<<<<<< HEAD
=======
int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

>>>>>>> origin/main
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
<<<<<<< HEAD
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
=======
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
>>>>>>> origin/main
		g_pid = 0;
		ret = parse(&script, &line_buf);
		if (ret == 1)
			continue ;
		else if (ret == 2)
		{
<<<<<<< HEAD
			rl_replace_line("exit", 0);
			rl_redisplay();
=======
			// write exit after prompt
>>>>>>> origin/main
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
<<<<<<< HEAD
	}
	script.termios_p.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &script.termios_p);
	return (0);
}

=======
		system("leaks minishell");
	}
}
>>>>>>> origin/main
