/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 14:58:48 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/07 13:29:51 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	return_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

/*
get_env_content(char *str, char **envp):
	This function iterates over the environment variables to find whether 
	or not the given variable (str) is defined and returns the content or 
	an empty freeable string.
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
	return (ret);
}
