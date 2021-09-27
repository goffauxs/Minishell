/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 14:38:46 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/27 12:09:00 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	open_redirs(t_token *head, t_redirection *redir)
{
	int	fd;

	redir->name = ft_trim_quotes(head->next->content);
	if (!ft_strncmp(head->content, ">>", 2))
		redir->flag = (O_CREAT | O_APPEND | O_RDWR);
	else if (!ft_strncmp(head->content, "<<", 2))
		redir->flag = -1;
	else if (!ft_strncmp(head->content, ">", 1))
		redir->flag = (O_CREAT | O_TRUNC | O_RDWR);
	else if (!ft_strncmp(head->content, "<", 1))
		redir->flag = O_RDONLY;
	fd = open(redir->name, redir->flag, 0644);
	close(fd);
}

static void	parse_commands(t_token *head, t_command *commands)
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
				open_redirs(head, &commands[i].in);
			else if (head->type == TOKEN_REDIR_OUT)
				open_redirs(head, &commands[i].out);
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

int	parse(t_script *script, char **line_buf)
{
	t_token	*head;

	head = NULL;
	*line_buf = readline("Minishell > ");
	if (!*line_buf)
		return (2);
	add_history(*line_buf);
	if (!tokenizer(*line_buf, &head))
	{
		return_error("Syntax error\n");
		free(*line_buf);
		free_tokens(head);
		return (1);
	}
	replace_env_var(head, script);
	script->cmd_count = get_cmd_count(*line_buf);
	script->commands = malloc(sizeof(t_command) * script->cmd_count);
	set_filenames_null(script->commands, script->cmd_count);
	get_num_args(head, script);
	parse_commands(head, script->commands);
	free_tokens(head);
	free(*line_buf);
	return (0);
}
