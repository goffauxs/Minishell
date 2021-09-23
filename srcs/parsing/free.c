/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:08:33 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/21 16:30:03 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_script *script)
{
	int	i;
	int	j;

	i = -1;
	while (++i < script->cmd_count)
	{
		j = -1;
		while (++j < script->commands[i].argc)
			free(script->commands[i].argv[j]);
		if (script->commands[i].in.name)
			free(script->commands[i].in.name);
		if (script->commands[i].out.name)
			free(script->commands[i].out.name);
		free(script->commands[i].argv);
	}
	free(script->commands);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->content);
		free(head);
		head = tmp;
	}
}
