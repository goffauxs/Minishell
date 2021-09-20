#include "minishell.h"

char *get_prompt()
{
	char *prompt;
	char **split_path;
	char cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	if(cwd[1] == '\0')
	{
		prompt = ft_strjoin("/", " > ");
		return(prompt);
	}
	else
	{
		split_path = ft_split(cwd, '/');
		int i = 0;
		while (split_path[i + 1])
			i++;
		prompt = ft_strjoin(split_path[i], " > ");
		return(prompt);
	}
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
	glo.exit_status = 0;
	glo.running_pid = 0;
	(void)argc;
	(void)argv;
	script.envp = envp;
	int ret;
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
			// rl_redisplay();
			// rl_on_new_line();
			// write(1, "exit\n", 5);
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
		ret = 0;
		ret = handle_cmd(script);
		if(ret == 8)
			break;
	}
}
