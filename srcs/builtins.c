#include "minishell.h"

int builtin_pwd()
{
	char	*buff;

	buff = (char *)malloc(sizeof(char) * MAX_PATH_LEN);
	if (!buff)
		return (1);
	buff = getcwd(buff, MAX_PATH_LEN);
	printf("%s\n", buff);
	free(buff);
}