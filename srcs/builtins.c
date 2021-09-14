#include "../includes/minishell.h"

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

int	builtin_env(char **envp)
{
	int	i;

	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
		printf("%s\n", envp[i++]);
	return (0);
}

int	builtin_export(t_script *script)
{
	char	**tmp;
	int		i;
	int		end;
	int		cursor;

	if (!script->envp)
		return (1);
	i = 0;
	while (script->envp[i])
		i++;
	tmp = script->envp;
	script->envp = (char **)malloc(sizeof(char *) * i + 2);
	end = i;
	i = 0;
	while (tmp[i])
	{
		script->envp[i] = (char *)malloc(sizeof(char) * ft_strlen(tmp[i]) + 1);
		cursor = 0;
		while (tmp[i][cursor])
		{
			script->envp[i][cursor] = tmp[i][cursor];
			cursor++;
		}
		script->envp[i][cursor] = '\0';
		i++;
	}
	script->envp[i++] = NULL; //replace with exported var
	script->envp[i] = NULL;
	// printf("%s\n", script->envp[i]);
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_script	script;

// 	(void)argc;
// 	(void)argv;
// 	script.envp = envp;
// 	builtin_export(&script);
// 	builtin_env(script.envp);
// 	return (0);
// }
