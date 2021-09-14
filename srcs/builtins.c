#include "minishell.h"
#include "libft.h"

int builtin_echo(t_command *test)
{
	int i;
	size_t flag;

	i = 1;
	flag = 0;
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

int	builtin_cd(t_command *test)
{
	int i;
	char *home;

	i = 1;
	home = getenv("HOME");
	if (!test->argv[i + 1])
		return(chdir(home));
	i++;
	return(chdir(test->argv[i]));
}

void builtin_exit()
{
	exit(EXIT_SUCCESS);
}

int builtin_pwd()
{
	char	*buff;

	buff = (char *)malloc(sizeof(char) * MAX_PATH_LEN);
	if (!buff)
		return (1);
	buff = getcwd(buff, MAX_PATH_LEN);
	if (!buff)
		return (1);
	printf("%s\n", buff);
	free(buff);
	return (0);
}

int	builtin_env(char **envp) //a besoin de **envp du main
{
	int	i;

	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]);
	return (0);
}
