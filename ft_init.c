/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/16 17:33:55 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_exec(char *commands, t_pipex *pipex, char **envp)
{
	pipex->commands = ft_split(commands, ' ');
	pipex->paths = ft_path(envp);
	// if (!pipex->commands || !pipex->paths)
	// 	ft_error("error with command");
	// for (int i = 0; pipex->commands[i]; i++)
	// {
	// 	printf("%s\n", pipex->commands[i]);
	// }
	// for (int i = 0; pipex->paths[i]; i++)
	// {
	// 	printf("%s\n", pipex->paths[i]);
	// }
	
}

void	pipe_init(int ac, char **av, char **envp, t_pipex *pipex)
{
	pipe(pipex->fd);
	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
	{
		pipex->filein = open(av[1], O_RDONLY | O_CREAT, 0644);
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
		// ft_exec(av[3], pipex, envp);   // TO_DO
	}
}
