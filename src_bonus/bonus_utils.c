/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/22 01:28:01 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

void	free_fd(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->num_cmds - 1)
	{
		if (pipex->fd[i])
			free(pipex->fd[i]);
		i++;
	}
	if (pipex->fd)
		free(pipex->fd);
}

void	ft_exec(char *cmd, t_pipex *pipex, char **envp)
{
	pipex->commands = ft_split(cmd, ' ');
	pipex->paths = ft_get_paths(envp);
	pipex->command = ft_get_command(pipex->paths, pipex->commands);
	if (!pipex->command)
	{
		free_fd(pipex);
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("command not found");
	}
	if (execve(pipex->command, pipex->commands, envp) == -1)
	{
		free_fd(pipex);
		free(pipex->command);
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("execve error");
	}
}
