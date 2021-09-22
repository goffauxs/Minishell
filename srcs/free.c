#include "minishell.h"

void	free_commands(t_script *script)
{
	int	i;
	int	j;

	i = -1;
	while (++i < script->cmd_count)
	{
		j = -1;
		while (++j < script->commands[i].argc)
			free(script->commands[i].argv[j]);
		if (script->commands[i].in.name)
			free(script->commands[i].in.name);
		if (script->commands[i].out.name)
			free(script->commands[i].out.name);
		free(script->commands[i].argv);
	}
	free(script->commands);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->content);
		free(head);
		head = tmp;
	}
}
