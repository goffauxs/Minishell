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
	char	*ret;
	int		end;

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