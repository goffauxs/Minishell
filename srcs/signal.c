/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 18:56:59 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/10/04 15:12:41 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

void	sig_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		write(1, "Quit: 3\n", 8);
		rl_on_new_line();
		tputs(carriage_return, 1, ft_putchar);
	}
	else if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_handler_fork(int signum)
{
	if (signum == SIGQUIT)
	{
		write(1, "Quit: 3\n", 8);
		rl_on_new_line();
		tputs(carriage_return, 1, ft_putchar);
	}
	else if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
	}
}

void	sig_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_status = 1;
	}
}
