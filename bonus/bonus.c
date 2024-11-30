/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 20:37:29 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/30 03:25:16 by mtewelde         ###   ########.fr       */
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
		read_line = get_next_line(STDIN_FILENO);
		if (!read_line)
		{
			free(pipex);
			ft_error("error reading line ");
		}
		if (ft_strncmp(read_line, pipex->eof, ft_strlen(pipex->eof) == 0))
		{
			free(read_line);
			exit(0);
		}
		ft_putstr_fd(read_line, fd[1]);
		free(read_line);
	}
}

void	process_heredoc(t_pipex *pipex)
{
	int		fd[2];
	pid_t	pid;

	(void)pipex;
	if (pipe(fd) == -1)
		ft_error("pipe err : ");
	pid = fork();
	if (pid == -1)
		ft_error("for err : ");
	if (pid == 0)
	{
		get_heredoc(pipex, fd);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		wait(NULL);
	}
}

void	ft_init(int ac, char **av, t_pipex *pipex)
{
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
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
	{
		pipex->nb_cmd = ac - 3;
		pipex->index = 2;
		pipex->filein = open(av[1], O_RDONLY);
		pipex->fileout = open(av[ac -1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipex->filein == -1 || pipex->fileout == -1)
		{
			free(pipex);
			ft_error("error on opening file ");
		}
		if (dup2(pipex->filein, STDIN_FILENO) == -1)
			ft_error("error from dup2");
		close(pipex->filein);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	*pipex;

	if (ac < 5 || ((ft_strncmp(av[1], "here_doc", 8) == 0) && ac < 6))
	{
		ft_putstr_fd("arg error: ./pipex infile cmd1 ... cmd_n outfile \n", 1);
		exit(1);
	}
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_error("error with malloc");
	if (!envp)
		ft_error("error with envp");
	printf("%d\n", ac);
	ft_init(ac, av, pipex);
	free(pipex);
	return (0);
}
