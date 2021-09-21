/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 14:38:46 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/21 16:16:18 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_commands(t_token *head, t_command *commands)
{
	int			i;
	int			j;

	i = 0;
	while (head)
	{
		commands[i].argv = malloc(sizeof(char *) * (commands[i].argc + 1));
		j = 0;
		while (head && head->type != TOKEN_PIPE)
		{
			if (head->type == TOKEN_NAME)
				commands[i].argv[j++] = ft_trim_quotes(head->content);
			else if (head->type == TOKEN_REDIR_IN)
				commands[i].in.name = ft_trim_quotes(head->next->content);
			else if (head->type == TOKEN_REDIR_OUT)
				commands[i].out.name = ft_trim_quotes(head->next->content);
			if (head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_OUT)
				head = head->next;
			head = head->next;
		}
		if (head)
			head = head->next;
		commands[i].argv[j] = NULL;
		i++;
	}
}

static void	set_filenames_null(t_command *commands, int max)
{
	int	i;

	i = -1;
	while (++i < max)
	{
		commands[i].in.name = NULL;
		commands[i].out.name = NULL;
	}
}

int	parse(t_script *script)
{
	t_token	*head;
	char	*line_buf;

	head = NULL;
	line_buf = readline("Minishell > ");
	add_history(line_buf);
	if (!tokenizer(line_buf, &head))
	{
		return_error("Syntax error\n");
		free(line_buf);
		free_tokens(head);
		return (1);
	}
	replace_env_var(head, script->envp);
	script->cmd_count = get_cmd_count(line_buf);
	script->commands = malloc(sizeof(t_command) * script->cmd_count);
	set_filenames_null(script->commands, script->cmd_count);
	get_num_args(head, script);
	parse_commands(head, script->commands);
	free_tokens(head);
	free(line_buf);
	return (0);
}
