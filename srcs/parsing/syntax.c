/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:49:18 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/05 12:57:44 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_syntax_loop(t_token *head)
{
	if (!head->next && (head->type == TOKEN_PIPE
			|| head->type == TOKEN_REDIR_IN
			|| head->type == TOKEN_REDIR_OUT))
		return (return_error("Syntax error\n"));
	// if (head->next && head->next->type == TOKEN_NAME
	// 	&& (head->type != TOKEN_REDIR_IN
	// 		&& head->type != TOKEN_REDIR_OUT))
	// 	*cmd = 1;
	if (head->type == TOKEN_PIPE && head->next
		&& head->next->type == TOKEN_PIPE)
		return (return_error("Syntax error\n"));
	if (head->type == TOKEN_REDIR_OUT || head->type == TOKEN_REDIR_IN)
	{
		if (head->next && head->next->type != TOKEN_NAME)
			return (return_error("Syntax error\n"));
	}
	return (0);
}

int	check_syntax(t_token *head)
{
	// int		cmd;
	t_token	*tmp;

	tmp = head;
	// cmd = (head && head->type == TOKEN_NAME);
	if (head && head->type == TOKEN_PIPE)
		return (return_error("Syntax error\n"));
	while (head)
	{
		if (check_syntax_loop(head))
			return (1);
		head = head->next;
	}
	// if (!cmd && tmp)
	// 	return (return_error("Syntax error\n"));
	return (0);
}
