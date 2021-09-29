/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:15:17 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/29 09:59:45 by rvan-aud         ###   ########.fr       */
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
		ret = ft_strdup("");
		if (!ret)
			return (NULL);
	}
	return (ret);
}

static char	*get_env_var(char *str, char **envp, int *i)
{
	char	c;
	char	*tmp;

	*i = 0;
	if (*str == '?')
	{
		(*i)++;
		if (g_exit_status >= 256)
			return (ft_itoa(WEXITSTATUS(g_exit_status)));
		else
			return (ft_itoa(g_exit_status));
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
		ret = ft_strdup(*split);
		split++;
	}
	else
	{
		ret = ft_strdup("");
		if (!ret)
			return (NULL);
	}
	while (*split)
	{
		i = 0;
		tmp = ft_strjoin_free(ret,
				get_env_var(*split, script->envp, &i));
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
