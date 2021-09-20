/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/20 11:51:59 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd_count(char *line_buf)
{
	int		i;
	int		count;
	char	**split_str;

	count = 0;
	split_str = ft_split(line_buf, '|');
	while (split_str[count])
		count++;
	i = count;
	while (i-- > 0)
		free(split_str[i]);
	free(split_str);
	return (count);
}

int	return_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

int	set_redir_flag(char c, char chevron)
{
	if (chevron == '>' && c == '>')
		return (O_CREAT | O_APPEND | O_RDWR);
	else if (chevron == '>' && (c != '>' && c != '<'))
		return (O_CREAT | O_TRUNC | O_RDWR);
	else if (chevron == '<' && c == '<')
		return (0);
	else if (chevron == '<' && (c != '<' && c != '>'))
		return (O_RDWR);
	return (-1);
}

t_token	*create_token(const char *string, int size, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_substr(string, 0, size);
	token->size = size;
	token->type = type;
	token->next = NULL;
	return (token);
}

t_token	*last_token(t_token *head)
{
	t_token *tmp;

	if (!head)
		return (NULL);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	add_token_back(t_token **head, t_token *new_token)
{
	t_token	*tmp;

	if (new_token)
	{
		if (*head == NULL)
			*head = new_token;
		else
		{
			tmp = last_token(*head);
			tmp->next = new_token;
		}
	}
}

t_operations	search_token_type(const char *s)
{
	const t_operations	*ex_ops = ops;
	t_operations		blank;

	blank.op = 0;
	blank.size = 0;
	blank.type = 0;
	while (ex_ops && ex_ops->op)
	{
		if (!ft_strncmp(s, ex_ops->op, ex_ops->size))
			return (*ex_ops);
		ex_ops++;
	}
	return (blank);
}

int	tokenizer(char *str, t_token **head)
{
	t_operations	curr;
	char			*prev;

	prev = str;
	while (str && *str)
	{
		curr = search_token_type(str);
		if ((curr.op != 0 || *str == '\"' || *str == '\'') && prev != str)
			add_token_back(head, create_token(prev, str - prev, TOKEN_NAME));
		if (curr.op != 0)
		{
			str += curr.size;
			if (curr.type != TOKEN_EAT)
				add_token_back(head, create_token(curr.op, curr.size, curr.type));
			prev = str;
		}
		else if (*str == '\"' || *str == '\'')
		{
			++str;
			while (*str && *str != '\'' && *str != '\"')
				++str;
			if (!*str || (*str != '\'' && *str != '\"'))
				return (0);
			++str;
		}
		else
			++str;
	}
	if (prev != str)
		add_token_back(head, create_token(prev, str - prev, TOKEN_NAME));
	return (1);
}

void	parse_commands(t_token *head, t_script *script)
{
	t_token		*tmp;
	t_token		*prev;
	int			i;
	int			j;

	i = 0;
	while (head)
	{
		tmp = head;
		prev = tmp;
		script->commands[i].argc = 0;
		while (tmp && tmp->type == TOKEN_NAME && (prev->type != TOKEN_REDIR_IN && prev->type != TOKEN_REDIR_OUT))
		{
			printf("%d ", tmp->type);
			prev = tmp;
			script->commands[i].argc++;
			tmp = tmp->next;
		}
		printf("command %d: %d\n", i, script->commands[i].argc);
		script->commands[i].argv = malloc(sizeof(char *) * script->commands[i].argc);
		j = 0;
		while (head && head->type != TOKEN_PIPE)
		{
			if (head->type == TOKEN_NAME)
				script->commands[i].argv[j++] = ft_strdup(head->content);
			if (head->type == TOKEN_REDIR_OUT || head->type == TOKEN_REDIR_IN)
			{
				if (head->type == TOKEN_REDIR_IN)
					script->commands[i].out.name = ft_strdup(head->next->content);
				else
					script->commands[i].in.name = ft_strdup(head->next->content);
				head = head->next;
			}
			head = head->next;
		}
		if (!head)
			break ;
		head = head->next;
		i++;
	}
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

int	main(void)
{
	t_script	script;
	t_token		*head;
	char		*line_buf;

	head = NULL;
	while (1)
	{
		line_buf = readline("Minishell > ");
		add_history(line_buf);
		if (!tokenizer(line_buf, &head))
		{
			return_error("Syntax error\n");
			continue ;
		}
		script.cmd_count = get_cmd_count(line_buf);
		script.commands = malloc(sizeof(t_command) * script.cmd_count);
		parse_commands(head, &script);
		for (int i = 0; i < script.cmd_count; i++)
		{
			printf("%s\n", script.commands[i].argv[0]);
			for (int j = 1; j < script.commands[i].argc; j++)
				printf("\t%s\n", script.commands[i].argv[j]);
		}
		for (int i = 0; i < script.cmd_count; i++)
		{
			for (int j = 0; j < script.commands[i].argc; j++)
				free(script.commands[i].argv[j]);
			if (script.commands[i].in.name)
				free(script.commands[i].in.name);
			if (script.commands[i].out.name)
				free(script.commands[i].out.name);
			free(script.commands[i].argv);
		}
		free(script.commands);
		free_tokens(head);
		head = NULL;
		if (!ft_strncmp(line_buf, "exit", 4))
		{
			free(line_buf);
			break ;
		}
		free(line_buf);
	}
	system("leaks minishell");
}
