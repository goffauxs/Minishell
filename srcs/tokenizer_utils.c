/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:12:47 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/21 16:40:27 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_operations	*fill_ex_ops(void)
{
	const t_operations	ops[] =
	{
		{">>", 2, TOKEN_REDIR_OUT},
		{"<<", 2, TOKEN_REDIR_IN},
		{"|", 1, TOKEN_PIPE},
		{">", 1, TOKEN_REDIR_OUT},
		{"<", 1, TOKEN_REDIR_IN},
		{" ", 1, TOKEN_EAT},
		{"\n", 1, TOKEN_EAT},
		{"\v", 1, TOKEN_EAT},
		{"\t", 1, TOKEN_EAT},
		{"\r", 1, TOKEN_EAT},
		{"\f", 1, TOKEN_EAT},
		{NULL, 1, 0}
	};

	return (ops);
}

int	treat_quotes(char **str)
{
	char	open_quote;

	open_quote = **str;
	++(*str);
	while (**str && **str != open_quote)
		++(*str);
	if (!**str || (**str != open_quote))
		return (0);
	return (1);
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
			while (*head)
				*head = (*head)->next;
			(*head)->next = new_token;
		}
	}
}

t_operations	search_token_type(const char *s)
{
	const t_operations	*ex_ops = fill_ex_ops();
	t_operations		blank;

	blank.op = 0;
	blank.size = 0;
	blank.type = 0;
	while (ex_ops && ex_ops->op)
	{
		if (!ft_strncmp(s, ex_ops->op, ex_ops->size))
			return (*ex_ops);
		ex_ops++;
	}
	return (blank);
}
