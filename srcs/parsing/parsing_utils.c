/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 14:54:29 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/06 15:05:50 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_blank_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		if (head->type != TOKEN_PIPE && head->next && !head->next->content[0])
		{
			tmp = head->next->next;
			free(head->next->content);
			free(head->next);
			head->next = tmp;
		}
		else
			head = head->next;
	}
}

void	set_filenames_null(t_command *commands, int max, t_token *head)
{
	int	i;

	i = -1;
	while (++i < max)
	{
		commands[i].in.name = NULL;
		commands[i].out.name = NULL;
	}
	while (head)
	{
		head->content = remove_quotes(head->content);
		head = head->next;
	}
}

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

int	get_cmd_count(t_token *head)
{
	int	count;

	count = 0;
	while (head)
	{
		if (!count)
			count = 1;
		if (head->type == TOKEN_PIPE)
			count ++;
		head = head->next;
	}
	return (count);
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
