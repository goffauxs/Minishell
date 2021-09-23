/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 16:15:17 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/23 14:59:28 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_env(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*get_env_var(char *str, char **envp, int *i)
{
	char	c;
	char	*tmp;

	*i = 0;
	(void)envp;
	// if (*str == '?')
	// 	return global exit code
	while (ft_is_env(str[*i]))
		(*i)++;
	c = str[*i];
	str[*i] = 0;
	tmp = getenv(str);
	str[*i] = c;
	if (!tmp)
	{
		tmp = malloc(sizeof(char));
		tmp[0] = 0;
	}
	return (tmp);
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
			before = "";
			if (head->content[0] != '$')
			{
				before = tmp_split[0];
				tmp_split++;
			}
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
