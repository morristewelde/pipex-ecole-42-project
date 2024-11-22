/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/22 01:28:27 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	ft_exec(char *commands, t_pipex *pipex, char **envp)
{
	pipex->commands = ft_split(commands, ' ');
	pipex->paths = ft_get_paths(envp);
	pipex->command = ft_get_command(pipex->paths, pipex->commands);
	if (pipex->command == NULL)
	{
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("command not found: ");
	}
	if (execve(pipex->command, pipex->commands, envp) == -1)
	{
		free(pipex->command);
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("execve error on commands : ");
	}
}

// pipex->filein = open(av[1], O_RDONLY | O_CREAT, 0644);
// close(pipex->fd[0])
// dup2(pipex->filein, STDIN_FILENO)
// close(pipex->filein)
// dup2(pipex->fd[1], STDOUT_FILENO)
// close(pipex->fd[1])
void	child_p1(char **av, t_pipex *pipex, char **envp)
{
	pipex->filein = open(av[1], O_RDONLY);
	if (pipex->filein < 0)
	{
		free(pipex);
		ft_error("Input file error ");
	}
	if (close(pipex->fd[0]) == -1)
		ft_error("sys error on close pipe read end ");
	if (dup2(pipex->filein, STDIN_FILENO) == -1)
		ft_error("System error on dup2 for filein ");
	if (close(pipex->filein) == -1)
		ft_error("System error on close filein ");
	if (dup2(pipex->fd[1], STDOUT_FILENO) == -1)
		ft_error("System error on dup2 for pipe write end ");
	if (close(pipex->fd[1]) == -1)
		ft_error("System error on close pipe write end ");
	ft_exec(av[2], pipex, envp);
}

// pipex->fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644)
// close(pipex->fd[1])
// dup2(pipex->fd[0], STDIN_FILENO)
// close(pipex->fd[0])
// dup2(pipex->fileout, STDOUT_FILENO)
// close(pipex->fileout)
void	child_p2(char **av, t_pipex *pipex, char **envp)
{
	pipex->fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->fileout < 0)
	{
		free(pipex);
		ft_error("Output file error ");
	}
	if (close(pipex->fd[1]) == -1)
		ft_error("System error on close pipe write end ");
	if (dup2(pipex->fd[0], STDIN_FILENO) == -1)
		ft_error("System error on dup2 for pipe read end");
	if (close(pipex->fd[0]) == -1)
		ft_error("System error on close pipe read end");
	if (dup2(pipex->fileout, STDOUT_FILENO) == -1)
		ft_error("System error on dup2 for fileout");
	if (close(pipex->fileout) == -1)
		ft_error("System error on close fileout");
	ft_exec(av[3], pipex, envp);
}

void	pipe_init(char **av, char **envp, t_pipex *pipex)
{
	if (pipe(pipex->fd) == -1)
		ft_error("pipe error ");
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		ft_error("Fork Error on child 1");
	if (pipex->pid1 == 0)
		child_p1(av, pipex, envp);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		ft_error("Fork Error on chlid 2");
	if (pipex->pid2 == 0)
		child_p2(av, pipex, envp);
}
