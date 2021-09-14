/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/14 16:17:54 by sgoffaux         ###   ########.fr       */
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

int	set_redir_flag(char c)
{
	if (c == '>')
		return (O_CREAT | O_APPEND | O_RDWR);
	else if (c == '<')
		return (0);
	return (O_CREAT | O_TRUNC | O_RDWR);
}

char	*get_inoutfile(char *line_buf, char chevron, t_redirection *redir)
{
	char	*ret;
	int		start;
	int		end;

	start = 0;
	while (line_buf[start] != chevron)
		start++;
	if (line_buf[start])
	{
		redir->flag = set_redir_flag(line_buf[start + 1]);
		while (line_buf[start] == chevron || ft_isspace(line_buf[start]))
			start++;
		end = start;
		while (!ft_isspace(line_buf[end]))
			end++;
		ret = ft_substr(line_buf, start, (end - start));
	}
	else
		return (NULL);
	return (ret);
}

t_command	parse_command(char *line_buf)
{
	t_command	cmd;
	int			count;
	char		*tmp;

	count = 0;
	tmp = ft_strtrim(line_buf, " \t");
	
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
	char *line_buf;
	char cwd[PATH_MAX];
	char **split_path;
	char *prompt;
	char **split_buf;

	while (1)
	{
		getcwd(cwd, PATH_MAX);
		split_path = ft_split(cwd, '/');
		int i = 0;
		while (split_path[i + 1])
			i++;
		prompt = ft_strjoin(split_path[i], " > ");
		line_buf = readline(prompt);
		add_history(line_buf);
		script.out.name = get_inoutfile(line_buf, '>', &script.out);
		script.in.name = get_inoutfile(line_buf, '<', &script.in);
		script.cmd_count = get_cmd_count(line_buf);
		script.commands = malloc(sizeof(t_command) * script.cmd_count);
		split_buf = ft_split(line_buf, '|');
		for (int i=0; i < script.cmd_count; i++)
		{
			script.commands[i] = parse_command(split_buf[i]);
			printf("command: '%s' | # of args: %d\n", script.commands[i].cmd, script.commands[i].argc);
		}
		printf("infile: '%s' (%x) | outfile: '%s' (%x)\n", script.in.name, script.in.flag, script.out.name, script.out.flag);
		if (!ft_strncmp(line_buf, "exit", 4))
			break;
	}
}