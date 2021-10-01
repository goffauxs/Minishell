/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 14:54:35 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/01 17:25:47 by sgoffaux         ###   ########.fr       */
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

void	copy_in_dquotes(char *start, char *end, char **str, int *i)
{
	while (start != end)
	{
		(*str)[*i] = *start;
		start++;
		(*i)++;
	}
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
	tmp[i] = '\0';
	ret = ft_strdup(tmp);
	free(tmp);
	free(copy);
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
