
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
char	*absolute_relative(const char *command, unsigned int slash);
char	*ft_get_command(char **paths, char **commands);
char	**ft_get_paths(char **envp);
void	ft_exec(char *cmd, t_pipex *pipex, char **envp);
void	free_fd(t_pipex *pipex);

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


void	ft_freestr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	count_str(char *str, char sep)
{
	int		result;
	int		flag;

	result = 0;
	while (*str)
	{
		flag = 0;
		while (*str == sep && *str)
			str++;
		while (*str != sep && *str)
		{
			if (!flag)
			{
				result++;
				flag = 1;
			}
			str++;
		}
	}
	return (result);
}

static char	*get_word(char *str, char sep)
{
	static int	index = 0;
	char		*result;
	int			len;
	int			i;

	len = 0;
	i = 0;
	while (str[index] == sep)
		index++;
	while ((str[index + len] != sep) && str[index + len])
		len++;
	result = malloc((size_t)len * sizeof(char) + 1);
	if (result == NULL)
		return (NULL);
	while ((str[index] != sep) && str[index])
		result[i++] = str[index++];
	result[i] = '\0';
	return (result);
}

char	**ft_split(char *str, char sep)
{
	int		i;
	int		words;
	char	**result;

	i = 0;
	words = count_str(str, sep);
	result = malloc(sizeof(char *) * (size_t)(words + 1));
	if (!words)
	{
		free(result);
		ft_error("malloc error on split");
	}
	if (result == NULL)
		return (NULL);
	while (words-- > 0)
		result[i++] = get_word(str, sep);
	result[i] = NULL;
	return (result);
}

char	*ft_get_command(char **paths, char **commands)
{
	int		i;
	char	*tmp;
	char	*res;

	i = 0;
	if (absolute_relative(commands[0], '/') != 0)
		return (commands[0]);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		res = ft_strjoin(tmp, commands[0]);
		free(tmp);
		if (access(res, F_OK) == 0)
		{
			ft_freestr(paths);
			return (res);
		}
		free(res);
		i++;
	}
	return (NULL);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] && s2[i])
	{
		if ((unsigned char) s1[i] != (unsigned char) s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
		i++;
	}
	return (0);
}

char	*absolute_relative(const char *command, unsigned int slash)
{
	int		i;
	char	s;

	s = (char) slash;
	i = 0;
	while (command[i])
	{
		if (command[i] == s)
			return ((char *)&command[i]);
		i++;
	}
	return (NULL);
}

char	**ft_get_paths(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	int		i;
	int		j;
	size_t	len;
	char	*res;

	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	res = malloc(sizeof(char) * (len));
	if (!res)
		return (NULL);
	while (s1[i])
		res[i++] = s1[j++];
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}


void	free_fd(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->num_cmds - 1)
	{
		if (pipex->fd[i])
			free(pipex->fd[i]);
		i++;
	}
	if (pipex->fd)
		free(pipex->fd);
}

void	ft_exec(char *cmd, t_pipex *pipex, char **envp)
{
	pipex->commands = ft_split(cmd, ' ');
	pipex->paths = ft_get_paths(envp);
	pipex->command = ft_get_command(pipex->paths, pipex->commands);
	if (!pipex->command)
	{
		free_fd(pipex);
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("command not found\n");
	}
	if (execve(pipex->command, pipex->commands, envp) == -1)
	{
		free_fd(pipex);
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		free(pipex);
		ft_error("execve error\n");
	}
}


void	handle_child_process(char *cmd, t_pipex *pipex, char **envp, int i)
{
	int	j;

	if (i == 0)
	{
		dup2(pipex->filein, STDIN_FILENO);
		dup2(pipex->fd[i][1], STDOUT_FILENO);
	}
	else if (i == pipex->num_cmds - 1)
	{
		dup2(pipex->fd[i - 1][0], STDIN_FILENO);
		dup2(pipex->fileout, STDOUT_FILENO);
	}
	else
	{
		dup2(pipex->fd[i - 1][0], STDIN_FILENO);
		dup2(pipex->fd[i][1], STDOUT_FILENO);
	}
	j = 0;
	while (j < pipex->num_cmds - 1)
	{
		close(pipex->fd[j][0]);
		close(pipex->fd[j][1]);
		j++;
	}
	ft_exec(cmd, pipex, envp);
}

void	close_wait(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->num_cmds - 1)
	{
		close(pipex->fd[i][0]);
		close(pipex->fd[i][1]);
		i++;
	}
	i = 0;
	while (i < pipex->num_cmds)
	{
		wait(NULL);
		i++;
	}
}

void	create_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	pipex->fd = malloc((pipex->num_cmds - 1) * sizeof(int *));
	if (!pipex->fd)
		ft_error("malloc error\n");
	while (i < pipex->num_cmds - 1)
	{
		pipex->fd[i] = malloc(2 * sizeof(int));
		if (pipe(pipex->fd[i]) == -1)
			ft_error("pipe error\n");
		i++;
	}
}

void	pipex_init(t_pipex *pipex, char **av, char **envp, int ac)
{
	int	i;

	pipex->filein = open(av[1], O_RDONLY);
	pipex->fileout = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->filein < 0 || pipex->fileout < 0)
	{
		free(pipex);
		ft_error("Input file error \n");
	}
	pipex->num_cmds = ac - 3;
	create_pipes(pipex);
	i = 0;
	while (i < pipex->num_cmds)
	{
		pipex->pid = fork();
		if (pipex->pid == -1)
			ft_error("fork error\n");
		if (pipex->pid == 0)
			handle_child_process(av[2 + i], pipex, envp, i);
		i++;
	}
	close_wait(pipex);
	free_fd(pipex);
}


void	ft_error(char *err)
{
	int	i;

	i = 0;
	while (err[i])
	{
		write(2, &err[i], 1);
		i++;
	}
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
		ft_error("arg error: ./pipex_bonus infile cmd1 ... cmd_n outfile \n");
	while (i < ac)
	{
		if (!white_space(av[i]))
			ft_error("error with argument! \n");
		i++;
	}
	if (!envp)
		ft_error("error with envp! \n");
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_error("error with malloc on initializing pipex \n");
	pipex_init(pipex, av, envp, ac);
	free(pipex);
	