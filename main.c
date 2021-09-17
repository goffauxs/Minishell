
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
	prompt = ft_strjoin(split_path[i], " > ");
	return(prompt);
}

int main(int argc, char **argv, char **envp)
{
	t_script script;
	int err;
	int i;
	char *line_buf;
	char *prompt;
	char **split_buf;
	// EXECUTION
	exit_status = 0;
	int	ret;
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
		script.cmd_count = get_cmd_count(line_buf);
		script.commands = malloc(sizeof(t_command) * script.cmd_count);
		split_buf = ft_split(line_buf, '|');
		while (i < script.cmd_count)
		{
			script.commands[i] = parse_command(split_buf[i]);
			//printf("command: '%s' | # of args: %d\n", script.commands[i].cmd, script.commands[i].argc);
			//printf("outfile: '%s' (%x)\n", script.commands[i].out.name, script.commands[i].out.flag);
			i++;
		}
		// EXECUTION
		i = 0;
		while (i < script.cmd_count)
		{
			ret = check_builtin(script.commands[i].cmd);
			if (ret == 0)
				handle_cmd(script, i);
			else
			{
				if(ret == 7)
				{
					if (!handle_builtin(ret, script, i))
					{
						ret ++;
						break;
					}
				}
				else
					handle_builtin(ret, script, i);
			}
			i++;
		}
		if(ret == 8)
			break;
	}
}
