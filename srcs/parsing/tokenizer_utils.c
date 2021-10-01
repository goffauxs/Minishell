/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:12:47 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/01 17:59:03 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	const t_operations		ex_ops[12] =
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
	const t_operations		blank = {0, 0, 0};
	int						i;

	i = -1;
	while (ex_ops[++i].op)
		if (!ft_strncmp(s, ex_ops[i].op, ex_ops[i].size))
			return (ex_ops[i]);
	return (blank);
}

int	get_double_quote_count(char *str)
{
	int		i;

	i = 0;
	while (str && *str)
	{
		str = ft_strnstr(str, "\"\"", ft_strlen(str));
		if (str)
		{
			str += 2;
			i++;
		}
	}
	return (i);
}

void	copy_in_dquotes(char *start, char *end, char **str, int *i)
{
	while (start != end)
	{
		(*str)[*i] = *start;
		start++;
		(*i)++;
	}
}
