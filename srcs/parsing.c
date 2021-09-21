/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 14:38:46 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/21 14:48:58 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd_count(char *line_buf)
{
	int		i;
	int		count;
	char	**split_str;

	if (!line_buf)
		return (0);
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

static int	treat_quotes(char **str)
{
	char	open_quote;

	open_quote = **str;
	++(*str);
	while (**str && **str != open_quote)
		++(*str);
	if (!**str || (**str != open_quote))
		return (0);
	return (1);
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
		else if ((*str == '\"' || *str == '\'') && !treat_quotes(&str))
			return (0);
		else
			++str;
	}
	if (prev != str)
		add_token_back(head, create_token(prev, str - prev, TOKEN_NAME));
	return (1);
}

static void	get_num_args(t_token *head, t_script *script)
{
	t_token	*tmp;
	int		i;

	i = 0;
	while (i < script->cmd_count)
	{
		script->commands[i].argc = 0;
		tmp = head;
		while (head && head->type != TOKEN_PIPE)
		{
			if (head->type == TOKEN_NAME && (tmp->type != TOKEN_REDIR_IN && tmp->type != TOKEN_REDIR_OUT))
				script->commands[i].argc++;
			tmp = head;
			head = head->next;
		}
		if (head)
			head = head->next;
		i++;
	}
}

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
				commands[i].argv[j++] = ft_strdup(head->content);
			else if (head->type == TOKEN_REDIR_IN)
				commands[i].in.name = ft_strdup(head->next->content);
			else if (head->type == TOKEN_REDIR_OUT)
				commands[i].out.name = ft_strdup(head->next->content);
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

int	ft_isenvv(char c)
{
	return ((c >= 65 && c <= 90) || ft_isdigit(c) || c == '_');
}

char	*get_env_var(char *str, char **envp, int *i)
{
	char	c;
	char	*tmp;
	int		len;

	*i = 0;
	if (ft_isdigit(*str))
		return ("");
	while (str[*i] && ft_isenvv(str[*i]))
		(*i)++;
	c = str[*i];
	str[*i] = 0;
	tmp = ft_strjoin(str, "=");
	str[*i] = c;
	len = ft_strlen(tmp);
	while (*envp)
	{
		if (!ft_strncmp(tmp, *envp, ft_strlen(tmp)))
		{
			free(tmp);
			return (*envp + len);
		}
		envp++;
	}
	free(tmp);
	return ("");
}

void	replace_env_var(t_token *head, char **envp)
{
	char	*tmp;
	char	**tmp_split;
	char	*before;
	int		i;

	while (head)
	{
		if (head->content[0] != '\'' && ft_strchr(head->content, '$'))
		{
			tmp_split = ft_split(head->content, '$');
			before = tmp_split[0];
			tmp_split++;
			while (*tmp_split)
			{
				i = 0;
				tmp = ft_strjoin(before, get_env_var(*tmp_split, envp, &i));
				before = ft_strjoin(tmp, (*tmp_split + i));
				free(tmp);
				tmp_split++;
			}
			free(head->content);
			head->content = before;
		}
		head = head->next;
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
	for (int i = 0; i < script->cmd_count; i++)
	{
		script->commands[i].in.name = NULL;
		script->commands[i].out.name = NULL;
	}
	get_num_args(head, script);
	parse_commands(head, script->commands);
	free_tokens(head);
	free(line_buf);
	return (0);
}