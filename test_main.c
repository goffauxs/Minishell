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
	int i;

	i = 0;
	test = testing_init(argc, argv);
	i = cd_built_in(test);
	printf("return_value = %d\n", i);
	perror("error : ");
	return(i);
}

// char * buf;
// buf = malloc(1000);
// buf = getcwd(buf, 1000);
// printf("IN MAIN VERIFICATION GETCWD |%s|\n", buf);