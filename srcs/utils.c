/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:58:48 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/04 15:13:08 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	return_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

void	termios(t_script *script)
{
	tcgetattr(STDIN_FILENO, &script->termios_p);
	script->termios_p.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &script->termios_p);
}
