#include "pipex_bonus.h"

void	ft_freestr(char **str);
size_t	ft_strlen(const char *str);
int	count_str(char *str, char sep);

static char	*get_word(char *str, char sep);
char	**ft_split(char *str, char sep);
int	ft_strncmp(const char *s1, const char *s2, size_t n);


char	*ft_strjoin(const char *s1, const char *s2);
void	ft_error(char *err);
void	ft_freestr(char **str);
char	*ft_get_command(char **paths, char **commands);
void	ft_exec(char *cmd, t_pipex *pipex, char **envp);
void	handle_child_process(char *cmd, t_pipex *pipex, char **envp, int i);
void	create_pipes(t_pipex *pipex);


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

void	ft_error(char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}


char	*ft_get_command(char **paths, char **commands)
{
	int		i;
	char	*tmp;
	char	*res;

	i = 0;
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

void	ft_exec(char *cmd, t_pipex *pipex, char **envp)
{
	pipex->commands = ft_split(cmd, ' ');
	pipex->paths = ft_get_paths(envp);
	pipex->command = ft_get_command(pipex->paths, pipex->commands);
	if (!pipex->command)
	{
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		ft_error("command not found");
	}
	if (execve(pipex->command, pipex->commands, envp) == -1)
	{
		free(pipex->command);
		ft_freestr(pipex->commands);
		ft_freestr(pipex->paths);
		ft_error("execve error");
	}
}

void	handle_child_process(char *cmd, t_pipex *pipex, char **envp, int i)
{
	if (i == 0) // First command
	{
		dup2(pipex->filein, STDIN_FILENO);
		dup2(pipex->fd[i][1], STDOUT_FILENO);
	}
	else if (i == pipex->num_cmds - 1) // Last command
	{
		dup2(pipex->fd[i - 1][0], STDIN_FILENO);
		dup2(pipex->fileout, STDOUT_FILENO);
	}
	else // Middle commands
	{
		dup2(pipex->fd[i - 1][0], STDIN_FILENO);
		dup2(pipex->fd[i][1], STDOUT_FILENO);
	}
	for (int j = 0; j < pipex->num_cmds - 1; j++) // Close all pipes
	{
		close(pipex->fd[j][0]);
		close(pipex->fd[j][1]);
	}
	ft_exec(cmd, pipex, envp);
}



void	create_pipes(t_pipex *pipex)
{
	pipex->fd = malloc((pipex->num_cmds - 1) * sizeof(int *));
	for (int i = 0; i < pipex->num_cmds - 1; i++)
	{
		pipex->fd[i] = malloc(2 * sizeof(int));
		if (pipe(pipex->fd[i]) == -1)
			ft_error("pipe error");
	}
}

void	pipex_init(t_pipex *pipex, char **av, char **envp, int ac)
{
	pipex->filein = open(av[1], O_RDONLY);
	pipex->fileout = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->filein < 0 || pipex->fileout < 0)
		ft_error("file error");
	pipex->num_cmds = ac - 3; // Total commands
	create_pipes(pipex);
	for (int i = 0; i < pipex->num_cmds; i++)
	{
		pipex->pid = fork();
		if (pipex->pid == -1)
			ft_error("fork error");
		if (pipex->pid == 0) // Child process
			handle_child_process(av[2 + i], pipex, envp, i);
	}
	// Close all pipes in the parent
	for (int i = 0; i < pipex->num_cmds - 1; i++)
	{
		close(pipex->fd[i][0]);
		close(pipex->fd[i][1]);
	}
	for (int i = 0; i < pipex->num_cmds; i++) // Wait for all child processes
		wait(NULL);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	if (ac < 5)
		ft_error("Usage: ./pipex infile cmd1 cmd2 ... cmdN outfile");
	pipex_init(&pipex, av, envp, ac);
	return (0);
}
