#include "minishell.h"
#include "libft.h"

int echo_built_in(t_command *test)
{
	int i;
	size_t flag;

	i = 1;
	flag = 0;
	if((ft_strncmp(test->argv[i], "echo", 4) == 0) && (ft_strlen(test->argv[i]) == 4))
	{
		if (!test->argv[i + 1])
		{
			printf("\n");
			return(0);
		}
		i++;
		if(test->argv[i][flag] == '-')
		{
			flag++;
			while(test->argv[i][flag] == 'n' || test->argv[i][flag] == 'e' || test->argv[i][flag] == 'E')
				flag++;
			if (flag == ft_strlen(test->argv[i]))
				i++;
			else
				flag = 0;
		}
		while(test->argv[i])
		{
			printf("%s", test->argv[i]);
			if(test->argv[i + 1])
				printf(" ");
			i++;
		}
		if(flag == 0)
			printf("\n");
		return(0);
	}
	return(-1);
}
