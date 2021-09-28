/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-aud <rvan-aud@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 14:54:29 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/09/28 17:20:34 by rvan-aud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_trim_quotes(char *str)
{
	char	*ret;
	char	c;
	int		newlen;

	c = str[0];
	newlen = ft_strlen(str) - 1;
	if (c == '\'' || c == '\"')
	{
		ret = malloc(sizeof(char) * newlen);
		if (!ret)
			return (NULL);
		ft_strlcpy(ret, (str + 1), newlen);
	}
	else
		ret = ft_strdup(str);
	return (ret);
}

static int	check_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

int	get_cmd_count(char *line_buf)
{
	int		i;
	int		count;
	char	**split_str;

	if (!line_buf)
		return (0);
	count = 0;
	split_str = ft_split(line_buf, '|');
	i = 0;
	while (split_str[i])
	{
		if (check_spaces(split_str[i]))
			count++;
		i++;
	}
	while (i-- > 0)
		free(split_str[i]);
	free(split_str);
	return (count);
}

int	return_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

void	get_num_args(t_token *head, t_script *script)
{
	t_token	*tmp;
	int		i;

	i = 0;
	while (i < script->cmd_count)
	{
		script->commands[i].argc = 0;
		tmp = head;
		while (head && head->type != TOKEN_PIPE)
		{
			if (head->type == TOKEN_NAME && (tmp->type != TOKEN_REDIR_IN
					&& tmp->type != TOKEN_REDIR_OUT))
				script->commands[i].argc++;
			tmp = head;
			head = head->next;
		}
		if (head)
			head = head->next;
		i++;
	}
}
