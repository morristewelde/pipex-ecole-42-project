/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/16 14:38:24 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <wait.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>

typedef struct s_pipex
{
	int		pid1;
	int		pid2;
	int		filein;
	int		fileout;
	int		fd[2];
	char	**cmd;
	char	**paths;
}			t_pipex;

//initialize pipex
void	pipe_init(int ac, char **av, char **envp, t_pipex *pipex);

//utils
char	**ft_path(char **envp);
void	ft_error(char *err);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char *str, char sep);

#endif
