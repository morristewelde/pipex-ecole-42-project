/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/18 00:20:37 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	absolute_relative(const char *command, unsigned int slash, int dot)
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
			return (1);
		i++;
	}
	return (0);
}

char	*ft_get_command(char **paths, char **commands)
{
	int		i;
	int		check_path;
	char	*tmp;
	char	*res;

	i = 0;
	check_path = absolute_relative(commands[0], '/', '.');
	if (check_path != 0)
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
		i++;
	}
	ft_freestr(paths);
	return (NULL);
}

void	ft_exec(char *commands, t_pipex *pipex, char **envp)
{
	pipex->commands = ft_split(commands, ' ');
	pipex->paths = ft_get_paths(envp);
	pipex->command = ft_get_command(pipex->paths, pipex->commands);
	if (pipex->command)
	{
		if (execve(pipex->command, pipex->commands, envp) == -1)
		{
			free(pipex->command);
			ft_freestr(pipex->commands);
			ft_error("execve error");
		}
	}
	else
	{
		free(pipex->command);
		ft_freestr(pipex->commands);
		ft_error("access error");
	}
}

void	pipe_init(char **av, char **envp, t_pipex *pipex)
{
	pipe(pipex->fd);
	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
	{
		pipex->filein = open(av[1], O_RDONLY);
		close(pipex->fd[0]);
		dup2(pipex->filein, STDIN_FILENO);
		close(pipex->filein);
		dup2(pipex->fd[1], STDOUT_FILENO);
		close(pipex->fd[1]);
		ft_exec(av[2], pipex, envp);
	}
	pipex->pid2 = fork();
	if (pipex->pid2 == 0)
	{
		pipex->fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		close(pipex->fd[1]);
		dup2(pipex->fd[0], STDIN_FILENO);
		close(pipex->fd[0]);
		dup2(pipex->fileout, STDOUT_FILENO);
		close(pipex->fileout);
		ft_exec(av[3], pipex, envp);
	}
}
