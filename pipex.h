/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/29 22:12:24 by mtewelde         ###   ########.fr       */
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
	char	**commands;
	char	**paths;
	char	*command;
}			t_pipex;

//initialize pipex
char	*absolute_relative(const char *command, unsigned int slash);
char	**ft_get_paths(char **envp);
char	*ft_get_command(char **paths, char **commands);
void	pipe_init(char **av, char **envp, t_pipex *pipex);

//utils
void	ft_error(char *err);
void	ft_freestr(char **str);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char *str, char sep);
char	*ft_strjoin(const char *s1, const char *s2);

#endif
