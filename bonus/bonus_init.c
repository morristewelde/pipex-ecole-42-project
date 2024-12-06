/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 20:37:29 by mtewelde          #+#    #+#             */
/*   Updated: 2024/12/06 19:27:33 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

void	get_heredoc(t_pipex *pipex, int fd[2])
{
	char	*read_line;

	close(fd[0]);
	while (1)
	{
		ft_putstr_fd("here_doc > ", 1);
		read_line = get_next_line(0);
		if (!read_line)
		{
			ft_error("error reading line ");
			break ;
		}
		if (ft_strncmp(read_line, pipex->eof, ft_strlen(pipex->eof)) == 0)
		{
			free(read_line);
			break ;
		}
		ft_putstr_fd(read_line, fd[1]);
		free(read_line);
	}
	close(fd[1]);
	free(pipex);
	exit(0);
}

void	process_heredoc(t_pipex *pipex)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		ft_error("pipe error : ");
	pid = fork();
	if (pid == -1)
		ft_error("fork error : ");
	if (pid == 0)
		get_heredoc(pipex, fd);
	else
	{
		wait(NULL);
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
}

void	multiple_command(int ac, char **av, t_pipex *pipex)
{
	pipex->nb_cmd = ac - 3;
	pipex->index = 2;
	pipex->filein = open(av[1], O_RDONLY);
	pipex->fileout = open(av[ac -1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->filein == -1)
	{
		free(pipex);
		ft_error("error on input file ");
	}
	if (pipex->fileout == -1)
	{
		free(pipex);
		ft_error("error on output file ");
	}
	if (dup2(pipex->filein, STDIN_FILENO) == -1)
		ft_error("error from dup2");
	close(pipex->filein);
}

void	ft_init_bonus(int ac, char **av, t_pipex *pipex)
{
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		pipex->nb_cmd = ac - 4;
		pipex->index = 3;
		pipex->eof = av[2];
		pipex->fileout = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (pipex->fileout == -1)
		{
			free(pipex);
			ft_error("error fileout ");
		}
		process_heredoc(pipex);
	}
	else
		multiple_command(ac, av, pipex);
}
