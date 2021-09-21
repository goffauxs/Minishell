#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_script	script;
	char 		*line_buf;

	(void)argc;
	(void)argv;
	script.envp = envp;
	line_buf = NULL;
	while (1)
	{
		if (parse(&script, &line_buf))
			continue ;
		if (script.cmd_count > 0)
			handle_cmd(script);
		free_commands(&script);
		if (!ft_strncmp(line_buf, "exit", 4) && script.cmd_count == 1)
		{
			free(line_buf); //aussi pour control d
			break ;
		}
	}
	//system("leaks minishell");
}

// int main(int argc, char **argv, char **envp)
// {
// 	t_script script;
// 	t_token		*head;
// 	int err;
// 	int i;
// 	char *line_buf;
// 	char *prompt;
// 	// char **split_buf;
// 	// EXECUTION
// 	exit_status = 0;
// 	// int	ret;
// 	(void)argc;
// 	(void)argv;
// 	script.envp = envp;
// 	//
// 	while (1)
// 	{
// 		// SIGNAUX
// 		signal(SIGQUIT, sig_handler);
// 		signal(SIGINT, sig_handler);
// 		//
// 		err = 0;
// 		i = 0;
// 		prompt = get_prompt();
// 		line_buf = readline(prompt);
// 		// SIGNAUX

// 		if (line_buf == NULL)
// 		{
// 			// rl_redisplay();
// 			// rl_on_new_line();
// 			// write(1, "exit\n", 5);
// 			break;
// 		}
// 		// if (line_buf == NULL)
// 		// {
// 		// 	rl_on_new_line ();
// 		// 	write(1, ft_strjoin(prompt, "exit\n"), ft_strlen(ft_strjoin(prompt, "exit\n")));
// 		// 	break;
// 		// }
// 		//
// 		add_history(line_buf);
// 		if (!tokenizer(line_buf, &head))
// 		{
// 			return_error("Syntax error\n");
// 			continue ;
// 		}
// 		script.cmd_count = get_cmd_count(line_buf);
// 		script.commands = malloc(sizeof(t_command) * script.cmd_count);
// 		for (int i = 0; i < script.cmd_count; i++)
// 		{
// 			script.commands[i].in.name = NULL;
// 			script.commands[i].out.name = NULL;
// 		}
// 		parse_commands(head, script.commands);
// 		for (int i = 0; i < script.cmd_count; i++)
// 		{
// 			script.commands[i].cmd = script.commands[i].argv[0];
// 			// printf("%s", script.commands[i].argv[0]);
// 			// printf("\t(in: '%s', out: '%s')\n", (script.commands[i].in.name) ? script.commands[i].in.name : "none", (script.commands[i].out.name) ? script.commands[i].out.name : "none");
// 			// for (int j = 1; j < script.commands[i].argc; j++)
// 			// 	printf("\t%s\n", script.commands[i].argv[j]);
// 		}
// 		if (!line_buf)
// 			break;
// 		// EXECUTION
// 		handle_cmd(script);
// 		for (int i = 0; i < script.cmd_count; i++)
// 		{
// 			for (int j = 0; j < script.commands[i].argc; j++)
// 				free(script.commands[i].argv[j]);
// 			if (script.commands[i].in.name)
// 				free(script.commands[i].in.name);
// 			if (script.commands[i].out.name)
// 				free(script.commands[i].out.name);
// 			free(script.commands[i].argv);
// 		}
// 		free(script.commands);
// 		free_tokens(head);
// 		head = NULL;
// 		if (!ft_strncmp(line_buf, "exit", 4) && script.cmd_count == 1)
// 		{
// 			free(line_buf);
// 			break ;
// 		}
// 		free(line_buf);
// 	}
// }