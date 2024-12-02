/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:08:02 by mtewelde          #+#    #+#             */
/*   Updated: 2024/12/02 21:48:41 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 3
# endif

typedef struct s_pipex
{
	int		fd[2];
	int		filein;
	int		fileout;
	int		pid;
	int		nb_cmd;
	int		index;
	char	**paths;
	char	**commands;
	char	*command;
	char	*eof;
}			t_pipex;

//utils
void	ft_error(char *err);
void	ft_putstr_fd(char *str, int fd);
int		white_space(char **av);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(const char *s1, const char *s2);
char	**ft_split(char *str, char sep);
size_t	ft_strlen(const char *str);
void	ft_freestr(char **str);

//pipex utils
char	*absolute_relative(const char *command, unsigned int slash);
char	*ft_get_command(char **paths, char **commands);
char	**ft_get_paths(char **envp);
void	ft_exec(char *cmd, t_pipex *pipex, char **envp);

//initialize bonus
void	ft_init_bonus(int ac, char **av, t_pipex *pipex);

//get next line
char	*get_next_line(int fd);
char	*ft_strdup(char *str);

#endif