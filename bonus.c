/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 20:37:29 by mtewelde          #+#    #+#             */
/*   Updated: 2024/12/03 23:42:21 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_exec(char *cmd, t_pipex *pipex, char **envp)
{
	pipex->commands = ft_split(cmd, ' ');
	pipex->paths = ft_get_paths(envp);
	pipex->command = ft_get_command(pipex->paths, pipex->commands);
	if (!pipex->command || !pipex->command)
	{
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("command not found");
	}
	if (execve(pipex->command, pipex->commands, envp) == -1)
	{
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("execve error ");
	}
}

void	middle_commads(char *command, char **envp, t_pipex *pipex)
{
	if (pipe(pipex->fd) == -1)
		ft_error("pipe error : ");
	pipex->pid = fork();
	if (pipex->pid == 0)
	{
		close(pipex->fd[0]);
		dup2(pipex->fd[1], STDOUT_FILENO);
		close(pipex->fd[1]);
		ft_exec(command, pipex, envp);
	}
	else
	{
		close(pipex->fd[1]);
		dup2(pipex->fd[0], STDIN_FILENO);
		close(pipex->fd[0]);
		wait(NULL);
	}
}

void	output_command(int ac, char **av, char **envp, t_pipex *pipex)
{
	pipex->pid = fork();
	if (pipex->pid == 0)
	{
		close(pipex->fd[0]);
		dup2(pipex->fd[1], STDOUT_FILENO);
		close(pipex->fd[1]);
		dup2(pipex->fileout, STDOUT_FILENO);
		ft_exec(av[ac - 2], pipex, envp);
	}
	else
	{
		close(pipex->fd[1]);
		dup2(pipex->fd[0], STDIN_FILENO);
		close(pipex->fd[0]);
		wait(NULL);
	}
}

int	invalid_arg(int ac, char **av, char **envp, t_pipex *pipex)
{
	int	i;

	i = ac - 1;
	if (ac < 5 || ((ft_strncmp(av[1], "here_doc", 8) == 0) && ac < 6)
		|| white_space(av) == 0)
	{
		if (((ft_strncmp(av[1], "here_doc", 8) == 0) && ac >= 6) || (ac >= 5))
			pipex->fileout = open(av[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		ft_putstr_fd("arg err: ./pipex_bonus infile cmd1...cmd_n outfile\n", 2);
		ft_putstr_fd("	or ./pipex_bonus here_doc EOF cmd cmd outfile\n", 2);
		return (1);
	}
	if (!envp)
	{
		ft_error("error with envp");
		return (1);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	*pipex;
	int		waitp;

	if (ac == 1)
	{
		ft_putstr_fd("arg err: ./pipex_bonus infile cmd1...cmd_n outfile\n", 2);
		exit(1);
	}
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_error("error with malloc");
	if (invalid_arg(ac, av, envp, pipex) || ac == 1)
	{
		free(pipex);
		exit (1);
	}
	ft_init_bonus(ac, av, pipex);
	while (pipex->index < ac - 2)
		middle_commads(av[pipex->index++], envp, pipex);
	output_command(ac, av, envp, pipex);
	waitp = wait(NULL);
	while (waitp != -1)
		waitp = wait(NULL);
	free(pipex);
	return (0);
}
