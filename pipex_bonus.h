#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	int		pid;
	int		filein;
	int		fileout;
	int		**fd;
	int		num_cmds;
	char	**commands;
	char	**paths;
	char	*command;
}			t_pipex;

//pipex init
void	handle_child_process(char *cmd, t_pipex *pipex, char **envp, int i);
void	create_pipes(t_pipex *pipex);
void	pipex_init(t_pipex *pipex, char **av, char **envp, int ac);

//pipex utils
char	*absolute_relative(const char *command, unsigned int slash, int dot);
char	*ft_get_command(char **paths, char **commands);
char	**ft_get_paths(char **envp);
void	ft_exec(char *cmd, t_pipex *pipex, char **envp);
void	free_resources(t_pipex *pipex);

//utils
void	ft_error(char *err);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		white_space(char *av);
char	*ft_strjoin(const char *s1, const char *s2);

//str_utils
char	**ft_split(char *str, char sep);
size_t	ft_strlen(const char *str);
void	ft_freestr(char **str);



#endif
