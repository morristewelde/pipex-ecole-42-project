/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/22 00:52:37 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

char	*absolute_relative(const char *command, unsigned int slash, int dot)
{
	int		i;
	char	d;
	char	s;

	d = (char) slash;
	s = (char) dot;
	i = 0;
	while (command[i])
	{
		if ((command[i] == d) || (command[i] == s))
			return ((char *)&command[i]);
		i++;
	}
	return (NULL);
}

char	*ft_get_command(char **paths, char **commands)
{
	int		i;
	char	*tmp;
	char	*res;

	i = 0;
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

void	ft_exec(char *cmd, t_pipex *pipex, char **envp)
{
	pipex->commands = ft_split(cmd, ' ');
	pipex->paths = ft_get_paths(envp);
	pipex->command = ft_get_command(pipex->paths, pipex->commands);
	if (!pipex->command)
	{
		free_resources(pipex);
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("command not found");
	}
	if (execve(pipex->command, pipex->commands, envp) == -1)
	{
		free_resources(pipex);
		free(pipex->command);
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("execve error");
	}
}
