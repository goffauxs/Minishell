/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:15:17 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/01 17:23:45 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static int	odd_before(char **str, int i, char c)
{
	int	count;
	int	j;

	count = 0;
	while (i--)
	{
		j = 0;
		while (str[i] && str[i][j])
		{
			if (str[i][j] == c)
				count++;
			j++;
		}
	}
	return (count % 2);
}

static int	odd_after(char **str, int i, char c)
{
	int	count;
	int	j;

	count = 0;
	while (str[i])
	{
		j = 0;
		while (str[i] && str[i][j])
		{
			if (str[i][j] == c)
				count++;
			j++;
		}
		i++;
	}
	return (count % 2);
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
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	c = str[*i];
	str[*i] = '\0';
	tmp = get_env_content(str, envp);
	str[*i] = c;
	return (tmp);
}

char	*replace_env_var_2(char *line_buf, char **envp)
{
	char	**split;
	int		i;
	int		j;
	char	*before;
	char	*tmp;

	i = 0;
	j = 0;
	split = ft_split(line_buf, '$');
	if (line_buf[0] != '$')
	{
		before = ft_strdup(split[0]);
		i++;
	}
	else
		before = ft_strdup("");
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

// static char	*get_env_var(char *str, char **envp, int *i)
// {
// 	char	c;
// 	char	*tmp;

// 	*i = 0;
// 	if (*str == '?')
// 	{
// 		(*i)++;
// 		if (g_exit_status >= 256)
// 			return (ft_itoa(WEXITSTATUS(g_exit_status)));
// 		else
// 			return (ft_itoa(g_exit_status));
// 	}
// 	while (ft_isalnum(str[*i]) || str[*i] == '_')
// 		(*i)++;
// 	c = str[*i];
// 	str[*i] = 0;
// 	tmp = get_env_content(str, envp);
// 	str[*i] = c;
// 	return (tmp);
// }

// static char	*env_loop(char **split, char **envp, t_token *head)
// {
// 	int		i;
// 	char	*tmp;
// 	char	*ret;

// 	if (head->content[0] != '$')
// 	{
// 		ret = ft_strdup(*split);
// 		split++;
// 	}
// 	else
// 	{
// 		ret = ft_strdup("");
// 		if (!ret)
// 			return (NULL);
// 	}
// 	while (*split)
// 	{
// 		i = 0;
// 		tmp = ft_strjoin_free(ret,
// 				get_env_var(*split, envp, &i));
// 		ret = ft_strjoin(tmp, (*split + i));
// 		free(tmp);
// 		split++;
// 	}
// 	return (ret);
// }

// void	replace_env_var(t_token *head, t_script *script)
// {
// 	char	**tmp_split;
// 	char	*before;

// 	while (head)
// 	{
// 		if (head->content[0] != '\'' && ft_strchr(head->content, '$'))
// 		{
// 			tmp_split = ft_split(head->content, '$');
// 			before = env_loop(tmp_split, script->envp, head);
// 			free_split(tmp_split);
// 			free(head->content);
// 			head->content = before;
// 		}
// 		head = head->next;
// 	}
// }
