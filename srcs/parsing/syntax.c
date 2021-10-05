/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:49:18 by mdeclerf          #+#    #+#             */
/*   Updated: 2021/10/05 15:27:12 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_syntax_loop(t_token *head)
{
	if (!head->next && (head->type == TOKEN_PIPE
			|| head->type == TOKEN_REDIR_IN
			|| head->type == TOKEN_REDIR_OUT))
		return (return_error("Syntax error\n"));
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
	t_token	*tmp;

	tmp = head;
	if (head && head->type == TOKEN_PIPE)
		return (return_error("Syntax error\n"));
	while (head)
	{
		if (check_syntax_loop(head))
			return (1);
		head = head->next;
	}
	return (0);
}
