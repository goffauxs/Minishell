/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/15 15:34:30 by sgoffaux         ###   ########.fr       */
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

int main()
{
	t_script script;
	int	err;
	char *line_buf;
	char cwd[PATH_MAX];
	char **split_path;
	char *prompt;
	char **split_buf;

	while (1)
	{
		err = 0;
		getcwd(cwd, PATH_MAX);
		split_path = ft_split(cwd, '/');
		int i = 0;
		while (split_path[i + 1])
			i++;
		prompt = ft_strjoin(split_path[i], " > ");
		line_buf = readline(prompt);
		add_history(line_buf);
		script.cmd_count = get_cmd_count(line_buf);
		script.commands = malloc(sizeof(t_command) * script.cmd_count);
		split_buf = ft_split(line_buf, '|');
		for (int i=0; i < script.cmd_count; i++)
		{
			script.commands[i] = parse_command(split_buf[i]);
		}
		if (!ft_strncmp(line_buf, "exit", 4))
			break;
	}
}