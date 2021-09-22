#include "minishell.h"

t_operations	add_element(const char *op, int size, t_token_type type)
{
	t_operations	tmp;

	tmp.op = op;
	tmp.size = size;
	tmp.type = type;
	return (tmp);
}

t_operations	*fill_ex_ops(void)
{
	t_operations	*ops;

	ops = malloc(sizeof(t_operations) * 12);
	if (!ops)
		return (NULL);
	ops[0] = add_element(">>", 2, TOKEN_REDIR_OUT);
	ops[1] = add_element("<<", 2, TOKEN_REDIR_IN);
	ops[2] = add_element("|", 1, TOKEN_PIPE);
	ops[3] = add_element(">", 1, TOKEN_REDIR_OUT);
	ops[4] = add_element("<", 1, TOKEN_REDIR_IN);
	ops[5] = add_element(" ", 1, TOKEN_EAT);
	ops[6] = add_element("\n", 1, TOKEN_EAT);
	ops[7] = add_element("\v", 1, TOKEN_EAT);
	ops[8] = add_element("\t", 1, TOKEN_EAT);
	ops[9] = add_element("\r", 1, TOKEN_EAT);
	ops[10] = add_element("\f", 1, TOKEN_EAT);
	ops[11] = add_element(NULL, 1, 0);
	return (ops);
}

t_token	*create_token(const char *string, int size, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = ft_substr(string, 0, size);
	token->size = size;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*tmp;

	if (new_token)
	{
		if (*head == NULL)
			*head = new_token;
		else
		{
			tmp = *head;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new_token;
		}
	}
}

t_operations	search_token_type(const char *s)
{
	t_operations		*ex_ops;
	t_operations		blank;

	ex_ops = fill_ex_ops();
	blank.op = 0;
	blank.size = 0;
	blank.type = 0;
	while (ex_ops && ex_ops->op)
	{
		if (!ft_strncmp(s, ex_ops->op, ex_ops->size))
			return (*ex_ops);
		ex_ops++;
	}
	//free(ex_ops);
	return (blank);
}
