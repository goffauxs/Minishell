/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/29 10:05:44 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**envp_malloc(char **envp)
{
	char	**tmp;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	tmp = malloc(sizeof(char *) * (i + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		tmp[i] = ft_strdup(envp[i]);
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

static void	main_loop(t_script *script, char **line_buf)
{
	int	ret;

	while (1)
	{
		script->cmd_count = 0;
		signal(SIGQUIT, SIG_IGN);
		ret = parse(script, line_buf);
		if (ret == 1)
			continue ;
		else if (ret == 2)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (script->cmd_count > 0)
		{
			if (handle_cmd(script))
				break ;
		}
		free_commands(script);
		system("leaks minishell");
	}
	if (script->cmd_count > 0)
		free_commands(script);
}

int	main(int argc, char **argv, char **envp)
{
	t_script	script;
	char		*line_buf;

	(void)argc;
	(void)argv;
	script.envp = envp_malloc(envp);
	line_buf = NULL;
	signal(SIGINT, sig_handler);
	termios(&script);
	main_loop(&script, &line_buf);
	free_path_env(script.envp);
	return (g_exit_status);
}
