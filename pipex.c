/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/12/01 23:41:22 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **envp)
{
	t_pipex	*pipex;

	if (ac != 5 || !white_space(av))
	{
		ft_putstr_fd("arg error ./pipex infile cmd1 cmd2 outfile \n", 2);
		exit(1);
	}
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
