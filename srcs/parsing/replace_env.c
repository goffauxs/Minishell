/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:15:17 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/04 16:41:28 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	c = str[*i];
	str[*i] = '\0';
	tmp = get_env_content(str, envp);
	str[*i] = c;
	return (tmp);
}

static char	**init_split_before(char *line_buf, char **before, int *i, int *j)
{
	char	**split;

	*i = 0;
	*j = 0;
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

char	*replace_env_var(char *line_buf, char **envp)
{
	char	**split;
	int		i;
	int		j;
	char	*before;
	char	*tmp;

	before = NULL;
	split = init_split_before(line_buf, &before, &i, &j);
	while (split[i])
	{
		if ((!odd_before(split, i, '\'') && !odd_after(split, i, '\''))
			|| (odd_before(split, i, '\"') && odd_after(split, i, '\"')))
		{
			j = 0;
			tmp = ft_strjoin_free(before, replace_loop(split[i], envp, &j));
			before = ft_strjoin(tmp, split[i] + j);
			free(tmp);
		}
		else
			before = ft_strjoin_free(before, ft_strjoin("$", split[i]));
		i++;
	}
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
