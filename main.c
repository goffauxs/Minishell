#include "minishell.h"

char *get_prompt()
{
	char *prompt;
	char **split_path;
	char cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	split_path = ft_split(cwd, '/');
	int i = 0;
	while (split_path[i + 1])
		i++;
	// prompt = ft_strjoin(split_path[i], " > ");
	prompt = "Kedulov > ";
	return(prompt);
}

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

// static int	treat_quotes()
// {
// 	// TODO
// }

int	tokenizer(char *str, t_token **head)
{
	t_operations	curr;
	char			*prev;
	// TODO: Add open_quote char to check that the same type of quotation mark closes the quotations

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
int	get_num_args(t_token *head)
{
	int		argc;
	t_token	*tmp;

	argc = 0;
	tmp = head;
	while (head && head->type != TOKEN_PIPE)
	{
		if (head->type == TOKEN_NAME && (tmp->type != TOKEN_REDIR_IN && tmp->type != TOKEN_REDIR_OUT))
			argc++;
		tmp = head;
		head = head->next;
	}
	return (argc);
}

void	parse_commands(t_token *head, t_command *commands)
{
	int			i;
	int			j;

	i = 0;
	while (head)
	{
		commands[i].argc = get_num_args(head);
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

int main(int argc, char **argv, char **envp)
{
	t_script script;
	t_token		*head;
	int err;
	int i;
	char *line_buf;
	char *prompt;
	// char **split_buf;
	// EXECUTION
	exit_status = 0;
	// int	ret;
	(void)argc;
	(void)argv;
	script.envp = envp;
	//
	while (1)
	{
		// SIGNAUX
		signal(SIGQUIT, sig_handler);
		signal(SIGINT, sig_handler);
		//
		err = 0;
		i = 0;
		prompt = get_prompt();
		line_buf = readline(prompt);
		// SIGNAUX
		if (line_buf == NULL)
		{
			rl_on_new_line ();
			write(1, ft_strjoin(prompt, "exit\n"), ft_strlen(ft_strjoin(prompt, "exit\n")));
			break;
		}
		//
		add_history(line_buf);
		if (!tokenizer(line_buf, &head))
		{
			return_error("Syntax error\n");
			continue ;
		}
		script.cmd_count = get_cmd_count(line_buf);
		script.commands = malloc(sizeof(t_command) * script.cmd_count);
		for (int i = 0; i < script.cmd_count; i++)
		{
			script.commands[i].in.name = NULL;
			script.commands[i].out.name = NULL;
		}
		parse_commands(head, script.commands);
		// for (int i = 0; i < script.cmd_count; i++)
		// {
		// 	printf("%s", script.commands[i].argv[0]);
		// 	// printf("\t(in: '%s', out: '%s')\n", (script.commands[i].in.name) ? script.commands[i].in.name : "none", (script.commands[i].out.name) ? script.commands[i].out.name : "none");
		// 	// for (int j = 1; j < script.commands[i].argc; j++)
		// 	// 	printf("\t%s\n", script.commands[i].argv[j]);
		// }
		// EXECUTION
		handle_cmd(script);
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
}