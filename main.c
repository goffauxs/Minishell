/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/16 16:16:45 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd_count(char *line_buf)
{
	int		count;
	char	*tmp;
	char	**split_str;

	count = 0;
	tmp = ft_strtrim(line_buf, " \t");
	split_str = ft_split(tmp, '|');
	while (split_str[count])
		count++;
	free(tmp);
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

int	get_inoutfile(char *line_buf, char chevron, t_redirection *redir, int start)
{
	char		*ret;
	int			end;

	while (line_buf[start] && line_buf[start] != chevron)
		start++;
	if (line_buf[start])
	{
		redir->flag = set_redir_flag(line_buf[start + 1], chevron);
		if (redir->flag == -1)
			return (1);
		while (line_buf[start] == chevron || ft_isspace(line_buf[start]))
			start++;
		end = start;
		while (line_buf[end] && !ft_isspace(line_buf[end]))
			end++;
		ret = ft_substr(line_buf, start, (end - start));
		redir->name = ret;
		redir->fd = open(ret, redir->flag, 0666);
		if (redir->fd < 0)
			return (1);
		close(redir->fd);
	}
	if (line_buf[start] != '\0' && line_buf[end] != '\0')
		return (get_inoutfile(line_buf, chevron, redir, start));
	return (0);
}

char	*trim_infile(char *str)
{
	int		i;
	int		len;
	char	*ret;

	i = 0;
	len = ft_strlen(str);
	while (str[i] && str[i] != '<')
		i++;
	while (str[i] == '<' || ft_isspace(str[i]))
		i++;
	while (str[i] && !ft_isspace(str[i]))
		i++;
	while (str[i] && ft_isspace(str[i]))
		i++;
	ret = ft_substr(str, i, len - i);
	return (ret);
}

char	*trim_outfile(char *str)
{
	int		i;
	int		len;
	char	*ret;

	i = 0;
	len = ft_strlen(str);
	while (str[i] && str[i] != '>')
		i++;
	while (ft_isspace(str[i - 1]))
		i--;
	ret = ft_substr(str, 0, i);
	return (ret);
}

t_token	*create_token(char *string, t_token_state state)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->string = string;
	token->state = state;
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

void	add_token_back(t_token *head, t_token *new_token)
{
	t_token	*tmp;

	if (new_token)
	{
		if (head == NULL)
			head = new_token;
		else
		{
			tmp = last_token(head);
			tmp->next = new_token;
		}
	}
}

t_token	*tokenizer(char *line_buf)
{
	char			*p;
	char			*token_string;
	t_token			*head;
	t_token_state	token_state;
	t_pointer_state	state;

	p = line_buf;
	state = START;
	head = NULL;
	token_state = DEFAULT;
	while (*p != '\0')
	{
		if (state == START)
		{
			while (ft_isspace(*p))
				p++;
			if (*p == '\'' || *p == '\"')
			{
				state = IN_STRING;
				token_string = p + 1;
				if (*p == '\'')
					token_state = QUOTES;
				else
					token_state = DQUOTES;
			}
			// else if (*p == '|')
			// {
				
			// }
			else
			{
				state = IN_WORD;
				token_string = p;
			}
		}
		else if (state == IN_STRING)
		{
			if ((token_state == QUOTES && *p == '\'') || (token_state == DQUOTES && *p == '\"'))
			{
				*p = '\0';
				if (!head)
					head = create_token(token_string, token_state);
				else
					add_token_back(head, create_token(token_string, token_state));
				state = START;
				token_state = DEFAULT;
			}
		}
		else if (state == IN_WORD)
		{
			if (ft_isspace(*p) || *p == '|')
			{
				*p = '\0';
				if (!head)
					head = create_token(token_string, token_state);
				else
				add_token_back(head, create_token(token_string, DEFAULT));
				state = START;
			}
		}
		p++;
	}
	return (head);
}

t_command	parse_command(char *split_buf)
{
	t_command	cmd;
	int			count;
	char		*tmp;

	cmd.in.name = NULL;
	cmd.out.name = NULL;
	count = 0;
	get_inoutfile(split_buf, '>', &cmd.out, 0);
	get_inoutfile(split_buf, '<', &cmd.in, 0);
	tmp = split_buf;
	if (cmd.in.name)
		tmp = trim_infile(tmp);
	if (cmd.out.name)
		tmp = trim_outfile(tmp);
	
	cmd.argv = ft_split(tmp, ' ');
	cmd.cmd = cmd.argv[0];
	while (cmd.argv[count])
		count++;
	cmd.argc = count;
	free(tmp);
	return (cmd);
}

int	main(void)
{
	t_script	script;
	t_token		*head;
	int			i;
	char		*line_buf;
	char		**split_buf;

	while (1)
	{
		line_buf = readline("Minishell > ");
		add_history(line_buf);
		head = tokenizer(line_buf);
		while (head)
		{
			printf("['%s', %s]", head->string, (head->state) ? (head->state == 1) ? "QUOTES" : "DQUOTES" : "DEFAULT");
			head = head->next;
		}
		printf("\n");
		script.cmd_count = get_cmd_count(line_buf);
		script.commands = malloc(sizeof(t_command) * script.cmd_count);
		split_buf = ft_split(line_buf, '|');
		i = -1;
		while (++i < script.cmd_count)
			script.commands[i] = parse_command(split_buf[i]);
		if (!ft_strncmp(line_buf, "exit", 4))
			break ;
	}
}
