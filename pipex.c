/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/18 00:21:14 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **envp)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_error("mallloc error for pipe!\n");
	if (ac != 5)
		ft_error("error with argument!\n");
	if (!envp)
		ft_error("error with envp!\n");
	pipe_init(av, envp, pipex);
	close(pipex->fd[0]);
	close(pipex->fd[1]);
	wait(NULL);
	wait(NULL);
	free(pipex);
	return (0);
}
