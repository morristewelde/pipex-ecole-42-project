/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/22 01:27:58 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
