/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:58:48 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/08 11:31:29 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_multiple_space(char *str)
{
	int 	i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	tmp = malloc(sizeof(char) * ft_strlen(str));
	if (!tmp)
		return (str);
	while (str[i])
	{
		if (ft_isspace(str[i]))
			tmp[j] = ' ';
		else
			tmp[j] = str[i];
		while (ft_isspace(str[i]) && str[i + 1] && ft_isspace(str[i + 1]))
			i++;
		i++;
		j++;
	}
	free(str);
	str = ft_strdup(tmp);
	free(tmp);
	return (str);
}

/*
fill_heredoc(t_redirection *file):
	This function creates a node for the heredoc linked list with the file name 
	as the content and adds it to the back of the list.
*/
void	fill_heredoc(t_redirection *file)
{
	t_list	*tmp;

	tmp = ft_lstnew(ft_strdup(file->name));
	if (file->heredoc == NULL)
		file->heredoc = tmp;
	else
		ft_lstadd_back(&file->heredoc, tmp);
	file->flag = -1;
}

/*
return_error(const char *msg, int system):
	This function prints an error message to standard error.
*/
int	return_error(const char *msg, int system)
{
	ft_putstr_fd("Minishell: ", 2);
	if (system)
		perror(msg);
	else
		ft_putendl_fd(msg, 2);
	return (1);
}

/*
get_env_content(char *str, char **envp):
	This function iterates over the environment variables to find whether 
	or not the given variable (str) is defined and returns the content or 
	an empty freeable string. This function is also called in the builtin
	pwd to set the env var PWD to the current directory.
*/
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
	return (replace_multiple_space(ret));
}