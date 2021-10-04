/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 14:38:46 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/04 16:13:10 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	open_redirs(t_token *head, t_redirection *redir)
{
	t_list	*tmp;

	if (redir->name)
		free(redir->name);
	if (!head->next || head->next->type != TOKEN_NAME)
		return ;
	redir->name = ft_strdup(head->next->content);
	if (!ft_strncmp(head->content, ">>", 2))
		redir->flag = (O_CREAT | O_APPEND | O_RDWR);
	else if (!ft_strncmp(head->content, "<<", 2))
	{
		tmp = ft_lstnew(ft_strdup(redir->name));
		if (!tmp)
			return ;
		if (redir->heredoc == NULL)
			redir->heredoc = tmp;
		else
			ft_lstadd_back(&redir->heredoc, tmp);
		redir->flag = -1;
	}
	else if (!ft_strncmp(head->content, ">", 1))
		redir->flag = (O_CREAT | O_TRUNC | O_RDWR);
	else if (!ft_strncmp(head->content, "<", 1))
		redir->flag = O_RDONLY;
	close(open(redir->name, redir->flag, 0644));
}

static void	parse_commands(t_token *head, t_command *commands, int i, int j)
{
	while (head)
	{
		commands[i].argv = malloc(sizeof(char *) * (commands[i].argc + 1));
		if (!commands[i].argv)
			return ;
		commands[i].in.heredoc = NULL;
		j = 0;
		while (head && head->type != TOKEN_PIPE)
		{
			if (head->type == TOKEN_NAME)
				commands[i].argv[j++] = ft_strdup(head->content);
			else if (head->type == TOKEN_REDIR_IN)
				open_redirs(head, &commands[i].in);
			else if (head->type == TOKEN_REDIR_OUT)
				open_redirs(head, &commands[i].out);
			if (head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_OUT)
				head = head->next;
			if (head)
				head = head->next;
		}
		if (head)
			head = head->next;
		commands[i].argv[j] = NULL;
		i++;
	}
}

static int	tokenize(char **line, t_token **head, t_script *s)
{
	char	*bis;

	bis = replace_env_var(*line, s->envp);
	if (!tokenizer(bis, head))
	{
		free_tokens(*head);
		free(bis);
		return (return_error("Syntax error\n"));
	}
	free(bis);
	return (0);
}

static int	check_syntax(t_token *head)
{
	int	cmd;

	cmd = (head && head->type == TOKEN_NAME);
	if (head && head->type == TOKEN_PIPE)
		return (return_error("Syntax error\n"));
	while (head)
	{
		if (head->next && head->next->type == TOKEN_NAME
			&& (head->type != TOKEN_REDIR_IN
				&& head->type != TOKEN_REDIR_OUT))
			cmd = 1;
		if (head->type == TOKEN_PIPE && head->next
			&& head->next->type == TOKEN_PIPE)
			return (return_error("Syntax error\n"));
		if (head->type == TOKEN_REDIR_OUT || head->type == TOKEN_REDIR_IN)
		{
			if (head->next && head->next->type != TOKEN_NAME)
				return (return_error("Syntax error\n"));
		}
		head = head->next;
	}
	if (!cmd && head)
		return (return_error("Syntax error\n"));
	return (0);
}

int	parse(t_script *script, char **line_buf)
{
	t_token	*head;

	head = NULL;
	*line_buf = readline("\033[0;32mMinishell > \033[0m");
	if (!*line_buf)
		return (2);
	add_history(*line_buf);
	if (tokenize(line_buf, &head, script))
		return (1);
	if (check_syntax(head))
	{
		free_tokens(head);
		free(*line_buf);
		return (1);
	}
	script->cmd_count = get_cmd_count(head);
	script->commands = malloc(sizeof(t_command) * script->cmd_count);
	if (!script->commands || script->cmd_count <= 0)
		return (1);
	set_filenames_null(script->commands, script->cmd_count, head);
	get_num_args(head, script);
	parse_commands(head, script->commands, 0, 0);
	free_tokens(head);
	free(*line_buf);
	return (0);
}
