#include "minishell.h"

int builtin_pwd()
{
	char	*buff;

	buff = (char *)malloc(sizeof(char) * 4096);
	if (!buff)
		return (1);
	buff = getcwd(buff, 4096);
	printf("%s\n", buff);
	free(buff);
}