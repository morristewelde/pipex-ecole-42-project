/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/22 00:52:24 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

void	free_resources(t_pipex *pipex)
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

void	handle_child_process(char *cmd, t_pipex *pipex, char **envp, int i)
{
	int	j;

	if (i == 0)
	{
		dup2(pipex->filein, STDIN_FILENO);
		dup2(pipex->fd[i][1], STDOUT_FILENO);
	}
	else if (i == pipex->num_cmds - 1)
	{
		dup2(pipex->fd[i - 1][0], STDIN_FILENO);
		dup2(pipex->fileout, STDOUT_FILENO);
	}
	else
	{
		dup2(pipex->fd[i - 1][0], STDIN_FILENO);
		dup2(pipex->fd[i][1], STDOUT_FILENO);
	}
	j = 0;
	while (j < pipex->num_cmds - 1)
	{
		close(pipex->fd[j][0]);
		close(pipex->fd[j][1]);
		j++;
	}
	ft_exec(cmd, pipex, envp);
}

void	close_wait(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->num_cmds - 1)
	{
		close(pipex->fd[i][0]);
		close(pipex->fd[i][1]);
		i++;
	}
	i = 0;
	while (i < pipex->num_cmds)
	{
		wait(NULL);
		i++;
	}
}

void	create_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	pipex->fd = malloc((pipex->num_cmds - 1) * sizeof(int *));
	if (!pipex->fd)
		ft_error("malloc error ");
	while (i < pipex->num_cmds - 1)
	{
		pipex->fd[i] = malloc(2 * sizeof(int));
		if (pipe(pipex->fd[i]) == -1)
			ft_error("pipe error");
		i++;
	}
}

void	pipex_init(t_pipex *pipex, char **av, char **envp, int ac)
{
	int	i;

	pipex->filein = open(av[1], O_RDONLY);
	if (pipex->filein < 0)
	{
		free(pipex);
		ft_error("Input file error ");
	}
	pipex->fileout = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->filein < 0 || pipex->fileout < 0)
		ft_error("file error");
	pipex->num_cmds = ac - 3;
	create_pipes(pipex);
	i = 0;
	while (i < pipex->num_cmds)
	{
		pipex->pid = fork();
		if (pipex->pid == -1)
			ft_error("fork error");
		if (pipex->pid == 0)
			handle_child_process(av[2 + i], pipex, envp, i);
		i++;
	}
	close_wait(pipex);
	free_resources(pipex);
}
