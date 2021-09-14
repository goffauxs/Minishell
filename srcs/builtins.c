#include "minishell.h"

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
