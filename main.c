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

int	get_outfile(char *line_buf, char chevron, t_redirection *redir)
{
	char		*ret;
	static int	start = 0;
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
		close(redir->fd);
	}
	if (line_buf[start] != '\0' && line_buf[end] != '\0')
		return (get_outfile(line_buf, chevron, redir));
	return (0);
}

t_command	parse_command(char *line_buf )
{
	t_command	cmd;
	int			count;
	char		*tmp;

	count = 0;
	tmp = ft_strtrim(line_buf, " \t");
	get_outfile(line_buf, '>', &cmd.out);
	cmd.argv = ft_split(tmp, ' ');
	cmd.cmd = cmd.argv[0];
	while (cmd.argv[count])
		count++;
	cmd.argc = count;
	free(tmp);
	return (cmd);
}

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
	prompt = ft_strjoin(split_path[i], " > ");
	return(prompt);
}

void	sig_handler(int signum)
{
	(void)signum;
	char * prompt;
	size_t i = 0;

	prompt = get_prompt();
	while(i < ft_strlen(prompt) + 2)
	{
		write(1, " \b", 2);
		if (i < ft_strlen(prompt))
			write(1, &prompt[i], 1);
		else if (i == ft_strlen(prompt))
			write(1, " ", 1);
		i++;
	}
	write(1, "\n",1);
	write(1, prompt, ft_strlen(prompt));
	signal(SIGINT, sig_handler);
}

char *new_prompt(char *line_buf, char *prompt)
{
	rl_on_new_line ();
	line_buf = readline(prompt);
	return(line_buf);
}

int main()
{
	t_script script;
	int	err;
	char *line_buf;
	char *prompt;
	char **split_buf;

	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_handler);
		err = 0;
		prompt = get_prompt();
		line_buf = readline(prompt);
		//
		if(line_buf == NULL)
		{
			write(1, "exit", 4);
			break;
		}
		//
		add_history(line_buf);
		script.cmd_count = get_cmd_count(line_buf);
		script.commands = malloc(sizeof(t_command) * script.cmd_count);
		split_buf = ft_split(line_buf, '|');
		for (int i=0; i < script.cmd_count; i++)
		{
			script.commands[i] = parse_command(split_buf[i]);
			//printf("command: '%s' | # of args: %d\n", script.commands[i].cmd, script.commands[i].argc);
			//printf("outfile: '%s' (%x)\n", script.commands[i].out.name, script.commands[i].out.flag);
		}
		if (!ft_strncmp(line_buf, "exit", 4))
			break;
	}
}

