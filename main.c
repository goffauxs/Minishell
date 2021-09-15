/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:26:41 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/15 14:14:00 by rvan-aud         ###   ########.fr       */
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

//
static char	*add_forw_slash(char *str)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_strdup(str);
	free(str);
	str = (char *)malloc(sizeof(char) * (ft_strlen(tmp) + 2));
	while (tmp[i])
	{
		str[i] = tmp[i];
		i++;
	}
	str[i++] = '/';
	str[i] = '\0';
	free(tmp);
	return (str);
}

static int	check_path_line(char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	return (i);
}

static void	init_vars(int *i, int *j)
{
	*i = 0;
	*j = 5;
}

static char	**split_paths(char **env)
{
	char	**path;
	char	*tmp;
	int		i;
	int		j;
	int		p_line;

	init_vars(&i, &j);
	p_line = check_path_line(env);
	path = ft_split(env[p_line], ':');
	tmp = ft_strdup(path[0]);
	free(path[0]);
	path[0] = (char *)malloc(sizeof(char) * ((ft_strlen(tmp) - 5) + 1));
	while (tmp[j])
		path[0][i++] = tmp[j++];
	path[0][i] = '\0';
	free(tmp);
	i = 0;
	while (path[i])
	{
		tmp = ft_strdup(path[i]);
		free(path[i]);
		path[i] = add_forw_slash(tmp);
		i++;
	}
	return (path);
}
//
//
static void	exec_cmd( char **path, char **cmd, char **env)
{
	char	*tmp;
	int		i;

	tmp = ft_strdup(*cmd);
	i = 0;
	while (execve(*cmd, cmd, env) == -1 && path[i])
	{
		free(*cmd);
		*cmd = ft_strjoin(path[i], tmp);
		if (!*cmd)
		{
			free(cmd);
			break ;
		}
		i++;
	}
}

void	child(char **path_env, t_script script, int i)
{
	exec_cmd(path_env, script.commands[i].argv, script.envp);
	printf("cmd doesn't exist\n");
	//free etc
}

void	handle_cmd(t_script script, int i)
{
	char	**path_env;
	int		pid;

	path_env = split_paths(script.envp);
	pid = fork();
	if (pid == -1)
		return ; //error
	if (pid == 0)
		child(path_env, script, i);
	wait(0);
	free(path_env);
}

int	check_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4))
		if (ft_strncmp(cmd, "cd", 2))
			if (ft_strncmp(cmd, "pwd", 3))
				if (ft_strncmp(cmd, "export", 6))
					if (ft_strncmp(cmd, "unset", 5))
						if (ft_strncmp(cmd, "env", 3))
							if (ft_strncmp(cmd, "exit", 4))
								return (0);
							else
								return (7);
						else
							return (6);
					else
						return (5);					
				else
					return (4);
			else
				return (3);
		else
			return (2);
	else
		return (1);
}

void	handle_builtin(int ret, t_script script, int i)
{
	(void)i;
	if (ret == 3)
		builtin_pwd();
	if (ret == 6)
		builtin_env(script.envp);
}
//

int main(int argc, char **argv, char **envp)
{
	t_script script;
	int	err;
	char *line_buf;
	char cwd[PATH_MAX];
	char **split_path;
	char *prompt;
	char **split_buf;
	//
	int	ret;

	(void)argc;
	(void)argv;
	script.envp = envp;
	//
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
		get_outfile(line_buf, '>', &script.out);
		script.cmd_count = get_cmd_count(line_buf);
		script.commands = malloc(sizeof(t_command) * script.cmd_count);
		split_buf = ft_split(line_buf, '|');
		for (int i=0; i < script.cmd_count; i++)
		{
			script.commands[i] = parse_command(split_buf[i]);
			printf("command: '%s' | # of args: %d\n", script.commands[i].cmd, script.commands[i].argc);
		}
		printf("infile: '%s' (%x) | outfile: '%s' (%x)\n", script.in.name, script.in.flag, script.out.name, script.out.flag);
		//
		i = 0;
		while (i < script.cmd_count)
		{
			ret = check_builtin(script.commands[i].cmd);
			if (ret == 0)
				handle_cmd(script, i);
			else
				handle_builtin(ret, script, i);
			i++;
		}
		//
		if (!ft_strncmp(line_buf, "exit", 4))
			break;
	}
}
