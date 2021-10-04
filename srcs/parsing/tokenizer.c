/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 14:54:35 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/04 11:21:40 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	treat_quotes(char **str)
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

char	*remove_double_quotes(char *str)
{
	int		i;
	int		j;
	char	*ret;

	i = get_double_quote_count(str);
	if (i == 0)
		return (ft_strdup(str));
	ret = malloc(sizeof(char) * (ft_strlen(str) - (i * 2) + 1));
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (*str && str[i])
	{
		if (str[i] == '\"' && str[i + 1] == '\"')
			i++;
		else
			ret[j++] = str[i];
		i++;
	}
	ret[j] = '\0';
	return (ret);
}

static char	*end_remove_quotes(char *tmp, char *copy, int i)
{
	char	*ret;

	tmp[i] = '\0';
	ret = ft_strdup(tmp);
	free(tmp);
	free(copy);
	return (ret);
}

char	*remove_quotes(char *str)
{
	char	*tmp;
	char	*ret;
	char	*copy;
	char	*start;
	int		i;

	i = 0;
	copy = str;
	tmp = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!tmp)
		return (NULL);
	while (str && *str)
	{
		if (*str == '\"')
		{
			start = str + 1;
			treat_quotes(&str);
			copy_in_dquotes(start, str, &tmp, &i);
		}
		else if (*str != '\'')
			tmp[i++] = *str;
		str++;
	}
	ret = end_remove_quotes(tmp, copy, i);
	return (ret);
}

int	tokenizer(char *str, t_token **head)
{
	t_operations	curr;
	char			*prev;

	prev = str;
	while (str && *str)
	{
		curr = search_token_type(str);
		if (curr.op != 0 && prev != str)
			add_token(head, create_token(prev, str - prev, TOKEN_NAME));
		if (curr.op != 0)
		{
			str += curr.size;
			if (curr.type != TOKEN_EAT)
				add_token(head, create_token(curr.op, curr.size, curr.type));
			prev = str;
		}
		else if ((*str == '\"' || *str == '\'') && !treat_quotes(&str))
			return (0);
		else
			++str;
	}
	if (prev != str)
		add_token(head, create_token(prev, str - prev, TOKEN_NAME));
	return (1);
}
