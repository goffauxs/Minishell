#include "minishell.h"

t_command *testing_init(int argc, char **argv)
{
	t_command *test;
	test = malloc(sizeof(t_command));
	test->cmd = NULL;
	test->argc = argc;
	test->argv = argv;
	return(test);
}

int main(int argc, char **argv)
{
	// char * rl_line_buffer;
	// while(1)
	// {
	// 	rl_line_buffer = readline(" Minishell> ");
	// 	if(! rl_line_buffer)
	// 		break;
	// 	add_history(rl_line_buffer);
	// 	free(rl_line_buffer);
	// }
	t_command *test;
	char * buf;
	int i;

	i = 1;
	test = testing_init(argc, argv);
	if((ft_strncmp(test->argv[i], "echo", 4) == 0) && (ft_strlen(test->argv[i]) == 4))
		builtin_echo(test);
	else if((ft_strncmp(test->argv[i], "cd", 2) == 0) && (ft_strlen(test->argv[i]) == 2))
	{
		buf = malloc(1000);
		buf = getcwd(buf, 1000);
		printf("BEFORE|%s|\n", buf);
		builtin_cd(test);
		buf = getcwd(buf, 1000);
		printf("AFTER|%s|\n", buf);
		free(buf);
	}
	else if ((ft_strncmp(test->argv[i], "exit", 2) == 0) && (ft_strlen(test->argv[i]) == 4))
		builtin_exit();
	else if ((ft_strncmp(test->argv[i], "pwd", 2) == 0) && (ft_strlen(test->argv[i]) == 3))
		builtin_pwd();
	free(test);
	return(0);
}
