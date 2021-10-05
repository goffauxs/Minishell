/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:15:17 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/05 15:28:49 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	first_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i] && !(str[i] == '\'' || str[i] == '\"'))
		i++;
	if (str[i] == '\'')
		return (1);
	return (0);
}

static char	*replace_loop(char *str, char **envp, int *i)
{
	char	*tmp;
	char	c;

	if (str[0] == '?')
	{
		(*i)++;
		if (g_exit_status >= 256)
			return (ft_itoa(WEXITSTATUS(g_exit_status)));
		else
			return (ft_itoa(g_exit_status));
	}
	if (ft_isspace(str[0]) || str[0] == '\'' || str[0] == '\"')
		return (ft_strdup("$"));
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	c = str[*i];
	str[*i] = '\0';
	tmp = get_env_content(str, envp);
	str[*i] = c;
	return (tmp);
}

static char	**init_split_before(char *line_buf, char **before, int *i)
{
	char	**split;

	split = ft_split(line_buf, '$');
	if (line_buf[0] && line_buf[0] != '$')
	{
		*before = ft_strdup(split[0]);
		(*i)++;
	}
	else
		*before = ft_strdup("");
	return (split);
}

char	*replace_env_var(char *line_buf, char **envp, int i, int j)
{
	char	**split;
	int		quotes;
	int		dquotes;
	char	*before;
	char	*tmp;

	before = NULL;
	split = init_split_before(line_buf, &before, &i);
	while (split[i])
	{
		quotes = (odd_before(split, i, '\'') && odd_after(split, i, '\''));
		dquotes = (odd_before(split, i, '\"') && odd_after(split, i, '\"'));
		if (quotes && (first_quote(line_buf) || !dquotes))
			before = ft_strjoin_free(before, ft_strjoin("$", split[i]));
		else
		{
			j = 0;
			tmp = ft_strjoin_free(before, replace_loop(split[i], envp, &j));
			before = ft_strjoin(tmp, split[i] + j);
			free(tmp);
		}
		i++;
	}
	if (line_buf[ft_strlen(line_buf) - 1] == '$' || !split[0])
		before = ft_strjoin_free(before, ft_strdup("$"));
	free_split(split);
	return (before);
}

char	*get_env_content(char *str, char **envp)
{
	char	*tmp;
	char	*ret;
	int		len;

	tmp = ft_strjoin(str, "=");
	len = ft_strlen(tmp);
	ret = NULL;
	while (*envp)
	{
		if (!ft_strncmp(tmp, *envp, len))
		{
			ret = ft_strdup(*envp + len);
			break ;
		}
		envp++;
	}
	free(tmp);
	if (!ret)
	{
		ret = ft_strdup("");
		if (!ret)
			return (NULL);
	}
	return (ret);
}
