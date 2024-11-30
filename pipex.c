/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/29 22:24:59 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

int	white_space(char *av)
{
	size_t	i;

	i = 0;
	if (!av)
		return (0);
	while (av[i] == 32 || (av[i] <= 13 && av[i] >= 9))
		i++;
	if (i == ft_strlen(av))
		return (0);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	*pipex;

	if (ac != 5 || !white_space(av[1]) || !white_space(av[2])
		|| !white_space(av[3]) || !white_space(av[4]))
		ft_error("arg error ./pipex infile cmd1 cmd2 outfile");
	if (!envp)
		ft_error("error with envp!");
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_error("mallloc error for pipe!");
	pipe_init(av, envp, pipex);
	close(pipex->fd[0]);
	close(pipex->fd[1]);
	wait(NULL);
	free(pipex);
	return (0);
}
