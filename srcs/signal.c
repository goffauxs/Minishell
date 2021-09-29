/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 18:56:59 by rvan-aud          #+#    #+#             */
/*   Updated: 2021/09/29 13:17:07 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signum)
{
	// printf("sig_handler\n");
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
		rl_redisplay();
	}
}

void	sig_handler_fork(int signum)
{
	// printf("sig_handler_fork\n");
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
	// printf("here\n");
	if (signum == SIGINT)
	{
		g_exit_status = 1;
	}
}
