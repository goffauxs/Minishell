#include "minishell.h"
#include "libft.h"

int echo_built_in(t_script *test)
{
	int i;
	size_t flag;

	i = 1;
	flag = 0;
	if((ft_strncmp(test->commands->argv[i], "echo", 4) == 0) && (ft_strlen(test->commands->argv[i]) == 4))
	{
		if (!test->commands->argv[i + 1])
		{
			printf("\n");
			return(0);
		}
		i++;
		if(test->commands->argv[i][flag] == '-')
		{
			flag++;
			while(test->commands->argv[i][flag] == 'n' || test->commands->argv[i][flag] == 'e' || test->commands->argv[i][flag] == 'E')
				flag++;
			if (flag == ft_strlen(test->commands->argv[i]))
				i++;
			else
				flag = 0;
		}
		while(test->commands->argv[i])
		{
			printf("%s", test->commands->argv[i]);
			if(test->commands->argv[i + 1])
				printf(" ");
			i++;
		}
		if(flag == 0)
			printf("\n");
		return(0);
	}
	return(-1);
}