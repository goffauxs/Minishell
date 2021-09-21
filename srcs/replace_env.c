/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:15:17 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/21 16:29:59 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_isenvv(char c)
{
	return ((c >= 65 && c <= 90) || ft_isdigit(c) || c == '_');
}

static char	*get_env_var(char *str, char **envp, int *i)
{
	char	c;
	char	*tmp;
	int		len;

	*i = 0;
	if (ft_isdigit(*str))
		return ("");
	while (str[*i] && ft_isenvv(str[*i]))
		(*i)++;
	c = str[*i];
	str[*i] = 0;
	tmp = ft_strjoin(str, "=");
	str[*i] = c;
	len = ft_strlen(tmp);
	while (*envp)
	{
		if (!ft_strncmp(tmp, *envp, ft_strlen(tmp)))
		{
			free(tmp);
			return (*envp + len);
		}
		envp++;
	}
	free(tmp);
	return ("");
}

void	replace_env_var(t_token *head, char **envp)
{
	char	*tmp;
	char	**tmp_split;
	char	*before;
	int		i;

	while (head)
	{
		if (head->content[0] != '\'' && ft_strchr(head->content, '$'))
		{
			tmp_split = ft_split(head->content, '$');
			before = tmp_split[0];
			tmp_split++;
			while (*tmp_split)
			{
				i = 0;
				tmp = ft_strjoin(before, get_env_var(*tmp_split, envp, &i));
				before = ft_strjoin(tmp, (*tmp_split + i));
				free(tmp);
				tmp_split++;
			}
			free(head->content);
			head->content = before;
		}
		head = head->next;
	}
}