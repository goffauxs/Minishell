/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeclerf <mdeclerf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:15:17 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/27 14:52:43 by mdeclerf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_getenv(char *str, char **envp)
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
		ret = malloc(sizeof(char));
		ret[0] = '\0';
	}
	return (ret);
}

static char	*get_env_var(char *str, char **envp, int *i, t_script *script)
{
	char	c;
	char	*tmp;

	*i = 0;
	if (*str == '?')
	{
		(*i)++;
		return (ft_itoa(script->exit_status));
	}
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	c = str[*i];
	str[*i] = 0;
	tmp = ft_getenv(str, envp);
	str[*i] = c;
	return (tmp);
}

static void	free_split(char **split)
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

static char	*env_loop(char **split, t_script *script, t_token *head)
{
	int		i;
	char	*tmp;
	char	*ret;

	if (head->content[0] != '$')
	{
		ret = ft_strdup(split[0]);
		split++;
	}
	else
	{
		ret = malloc(sizeof(char));
		ret[0] = '\0';
	}
	while (*split)
	{
		i = 0;
		tmp = ft_strjoin_free(ret,
				get_env_var(*split, script->envp, &i, script));
		ret = ft_strjoin(tmp, (*split + i));
		free(tmp);
		split++;
	}
	return (ret);
}

void	replace_env_var(t_token *head, t_script *script)
{
	char	**tmp_split;
	char	*before;

	while (head)
	{
		if (head->content[0] != '\'' && ft_strchr(head->content, '$'))
		{
			tmp_split = ft_split(head->content, '$');
			before = env_loop(tmp_split, script, head);
			free_split(tmp_split);
			free(head->content);
			head->content = before;
		}
		head = head->next;
	}
}
