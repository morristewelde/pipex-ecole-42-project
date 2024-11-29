/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 20:37:29 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/29 22:33:21 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

void	here_doc(int ac, char **av, t_pipex *pipex)
{

	printf("here doc");
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_error("error with malloc");
	if (ac < 5)
		ft_error("arg error: ./pipex_bonus infile cmd1 ... cmd_n outfile \n");
	if (!envp)
		ft_error("error with envp");
	here_doc(ac, av, pipex);
	return (0);
}