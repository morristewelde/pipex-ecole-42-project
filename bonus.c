/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/22 00:25:05 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int ac, char **av, char **envp)
{
	t_pipex	*pipex;
	int		i;

	i = 1;
	if (ac < 5)
		ft_error("arg error: ./pipex_bonus infile cmd1 ... cmd_n outfile");
	while (i < ac)
	{
		if (!white_space(av[i]))
			ft_error("error with argument!");
		i++;
	}
	if (!envp)
		ft_error("error with envp!");
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_error("error with malloc on initializing pipex ");
	pipex_init(pipex, av, envp, ac);
	free(pipex);
	return (0);
}
