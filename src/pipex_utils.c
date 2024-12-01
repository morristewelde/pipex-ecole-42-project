/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:23:35 by mtewelde          #+#    #+#             */
/*   Updated: 2024/12/01 20:53:35 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*ft_get_command(char **paths, char **commands)
{
	int		i;
	char	*tmp;
	char	*res;

	i = 0;
	if (absolute_relative(commands[0], '/') != 0)
		return (commands[0]);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		res = ft_strjoin(tmp, commands[0]);
		free(tmp);
		if (access(res, F_OK) == 0)
		{
			ft_freestr(paths);
			return (res);
		}
		free(res);
		i++;
	}
	return (NULL);
}

char	*absolute_relative(const char *command, unsigned int slash)
{
	int		i;
	char	s;

	s = (char) slash;
	i = 0;
	while (command[i])
	{
		if (command[i] == s)
			return ((char *)&command[i]);
		i++;
	}
	return (NULL);
}

char	**ft_get_paths(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

void	ft_error(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}
