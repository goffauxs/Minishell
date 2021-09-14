#include "minishell.h"

t_script *testingecho_init(t_script * test, int argc, char **argv)
{
	test->commands = malloc(sizeof(t_command));
	test->commands->cmd = NULL;
	test->commands->argc = argc;
	test->commands->argv = argv;
	test->outfile = NULL;
	test->infile = NULL;
	test->err = NULL;
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
	t_script *test;
	int i;

	i = 0;
	test = malloc(sizeof(t_script));
	test = testingecho_init(test, argc, argv);
	i = echo_built_in(test);
	//printf("return_value = %d", i);
	return(i);
}
