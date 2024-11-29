#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# ifndef OPEN_MAX
#  define  OPEN_MAX 1024
# endif

# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>

typedef struct s_bpipex
{
	int		fd[2];
	pid_t	pid;
	int		filein;
	int		fileout;
	char	**split_cmd;
	char	**chec;
	char	*result;
	int		i;
	char	*limiter;
}		t_bpipex;

int		ft_open_files(char *av, int i);
void	ft_error_message(char *mess);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_get_path(char **env);
char	*check_cmd(char **envp, char **cmd);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);
void	ft_putstr_fd(char *s, int fd);
char	*ft_sersh_for_newline(const char *s, int c);
char	*get_next_line(int fd);
char	*ft_searsh_for_slash(const char *s, int c);
char	*ft_strdup(const char *src);
char	*ft_substr(const char *s, unsigned int start, size_t len);
void	process_here_doc(int ac, char **av, t_bpipex *pipex);
void	last_cmd(int ac, char **av, char **env, t_bpipex *pipex);
void	first_middle_cmds(char *command, char **envp, t_bpipex *bpipe);
void	ft_execve(char **cmd, char **envp, t_bpipex *bpipe);
#endif

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if ((unsigned char) s1[i] != (unsigned char) s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
		i++;
	}
	return (0);
}

void	ft_execve(char **cmd, char **envp, t_bpipex *bpipe)
{
	bpipe->split_cmd = ft_split(cmd[0], ' ');
	bpipe->chec = ft_get_path(envp);
	if (!bpipe->split_cmd[0] || !bpipe->chec)
		ft_error_message("command not found!");
	bpipe->result = check_cmd(bpipe->chec, bpipe->split_cmd);
	if (bpipe->result)
	{
		if (execve(bpipe->result, bpipe->split_cmd, envp) == -1)
			ft_error_message("error message from execve!");
	}
	else
		ft_error_message("error from access!");
}

int	ft_open_files(char *av, int i)
{
	int	fd;

	fd = -1;
	if (i == 0)
		fd = open(av, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (i == 1)
		fd = open(av, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (i == 2)
		fd = open(av, O_RDONLY, 0644);
	else if (i == -1)
		ft_error_message("Error from open!");
	return (fd);
}


void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}


static int	ft_countword(char const *str, char c)
{
	int	i;
	int	count;
	int	b;

	i = 0;
	count = 0;
	b = 0;
	while (str[i])
	{
		if (str[i] != c && b == 0)
		{
			b = 1;
			count++;
		}
		else if (str[i] == c)
			b = 0;
		i++;
	}
	return (count);
}

static void	ft_free_split(char **words)
{
	int	i;

	i = 0;
	while (words[i] != NULL)
	{
		free(words[i]);
		i++;
	}
	free(words);
}

char	**ft_split(char const *s, char c)
{
	int		wc;
	int		wi;
	int		wl;
	char	**words;

	if (!s)
		return (NULL);
	wi = 0;
	wc = ft_countword(s, c);
	words = (char **)malloc (sizeof(char *) * (wc + 1));
	if (words == NULL)
		return (NULL);
	while (wi < wc)
	{
		while (*s == c)
			s++;
		wl = 0;
		while (s[wl] && s[wl] != c)
			wl++;
		words[wi] = ft_substr(s, 0, wl);
		if (!words[wi++])
			return (ft_free_split(words), NULL);
		s += wl;
	}
	return (words[wi] = NULL, words);
}


char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*result;
	size_t	lentgh_s1;
	size_t	lentgh_s2;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	lentgh_s1 = ft_strlen(s1);
	lentgh_s2 = ft_strlen(s2);
	result = (char *) malloc((lentgh_s1 + lentgh_s2 + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	while (s1[i] != '\0')
		result[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		result[j++] = s2[i++];
	result[j] = '\0';
	return (result);
}


char	*ft_strdup(const char *src)
{
	char	*dst;
	size_t	len;
	size_t	i;

	len = ft_strlen(src);
	dst = malloc(sizeof(char) * (len + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*find_line(int fd, char *rest, char *buffer)
{
	ssize_t		read_char;
	char		*tmp;

	while (!ft_sersh_for_newline(buffer, '\n'))
	{
		read_char = read(fd, buffer, BUFFER_SIZE);
		if (read_char <= 0)
			break ;
		buffer[read_char] = '\0';
		if (!rest)
			rest = ft_strdup(buffer);
		else
		{
			tmp = rest;
			rest = ft_strjoin(tmp, buffer);
			free (tmp);
		}
	}
	free (buffer);
	return (rest);
}

char	*fix_line(char *bufl)
{
	char	*rest;
	size_t	i;

	i = 0;
	while (bufl[i] != '\0' && bufl[i] != '\n')
		i++;
	rest = ft_substr(bufl, i + 1, ft_strlen(bufl) - i);
	if (!rest)
		return (NULL);
	if (bufl[i] != 0)
		bufl[i + 1] = '\0';
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*rest;
	char		*line;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX
		|| fd >= OPEN_MAX)
		return (free (rest), rest = NULL, NULL);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (free (rest), rest = NULL, NULL);
	buffer[0] = 0;
	line = find_line(fd, rest, buffer);
	if (!line)
		return (rest = NULL, NULL);
	rest = fix_line(line);
	if (!rest)
		return (free(line), NULL);
	if (line[0] == 0)
		return (free (rest), free (line), rest = NULL);
	buffer = ft_strdup(line);
	free(line);
	if (!buffer)
		return (free(rest), rest = NULL, NULL);
	return (buffer);
}


size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	str = (char *) malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_sersh_for_newline(const char *s, int c)
{
	unsigned int	i;
	char			chr;

	i = 0;
	chr = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == chr)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}


char	*ft_searsh_for_slash(const char *s, int c)
{
	unsigned int	i;
	char			chr;

	i = 0;
	chr = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == chr)
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}

void	ft_error_message(char *mess)
{
	int	i;

	i = 0;
	while (mess[i])
	{
		write (2, &mess[i], 1);
		i++;
	}
	write (2, "\n", 1);
	exit(EXIT_FAILURE);
}

void	ft_free_leak(char **str)
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

char	**ft_get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
			return (ft_split(env[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

char	*check_cmd(char **envp, char **cmd)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	if (ft_searsh_for_slash(cmd[0], '/') != 0)
		return (cmd[0]);
	else if (ft_searsh_for_slash(cmd[0], '.') != 0)
		return (cmd[0] + 2);
	while (envp[i])
	{
		tmp = ft_strjoin(envp[i], "/");
		tmp2 = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		if (access(tmp2, F_OK) == 0)
		{
			ft_free_leak(envp);
			return (tmp2);
		}
		free(tmp2);
		i++;
	}
	ft_free_leak(envp);
	return (NULL);
}



void	first_middle_cmds(char *command, char **envp, t_bpipex *bpipe)
{
	if (pipe(bpipe->fd) == -1)
		ft_error_message("Error from pipe");
	bpipe->pid = fork();
	if (bpipe->pid == -1)
		ft_error_message("Error from fork");
	if (bpipe->pid == 0)
	{
		close(bpipe->fd[0]);
		if (dup2(bpipe->fd[1], STDOUT_FILENO) < 0)
			ft_error_message("error from dup2 in fist_mid_cmd!");
		close(bpipe->fd[1]);
		ft_execve(&command, envp, bpipe);
	}
	else
	{
		close(bpipe->fd[1]);
		if (dup2(bpipe->fd[0], STDIN_FILENO) < 0)
			ft_error_message("error from dup2 in f_m parnt!");
		close(bpipe->fd[0]);
	}
}

void	last_cmd(int ac, char **av, char **env, t_bpipex *pipex)
{
	pipex->pid = fork();
	if (pipex->pid == -1)
		ft_error_message("Error from fork!");
	if (pipex->pid == 0)
	{
		close(pipex->fd[0]);
		dup2(pipex->fd[1], STDOUT_FILENO);
		close (pipex->fd[1]);
		dup2(pipex->fileout, STDOUT_FILENO);
		ft_execve(&av[ac - 2], env, pipex);
	}
	else
	{
		close(pipex->fd[1]);
		dup2(pipex->fd[0], STDIN_FILENO);
		close(pipex->fd[0]);
	}
}

void	ft_is_there_herdoc(t_bpipex *pipex, int fd[2])
{
	char	*ret_line;

	close(fd[0]);
	while (1)
	{
		ft_putstr_fd("here_doc> ", 1);
		ret_line = get_next_line(0);
		if (!ret_line)
			exit(EXIT_FAILURE);
		if (ft_strncmp(ret_line, pipex->limiter, ft_strlen(pipex->limiter)) == 0)
		{
			free(ret_line);
			exit(0);
		}

		ft_putstr_fd(ret_line, fd[1]);
		free(ret_line);
	}
}


void	ft_here_doc(t_bpipex *pipex)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		ft_is_there_herdoc(pipex, fd);
	else
	{
		close (fd[1]);
		if (dup2(fd[0], STDIN_FILENO) == -1)
			ft_error_message("error message from dup2 in here_doc!");
		close (fd[0]);
		wait (NULL);
	}
}

void	process_here_doc(int ac, char **av, t_bpipex *pipex)
{ 
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		pipex->limiter = ft_strjoin(av[2], "\n");
		pipex->i = 3;
		pipex->fileout = ft_open_files(av[ac - 1], 1);
		if (pipex->fileout < 0)
			ft_error_message("Error from fileout in heredoc!");
		if (ac < 6)
			ft_error_message("Error from ac in here_doc!");
		ft_here_doc(pipex);
	}
	else
	{
		pipex->i = 2;
		pipex->fileout = ft_open_files(av[ac - 1], 0);
		if (pipex->fileout < 0)
			ft_error_message("Error from fileout!");
		pipex->filein = ft_open_files(av[1], 2);
		if (pipex->filein < 0)
			ft_error_message("Error from filin!");
		if (dup2(pipex->filein, STDIN_FILENO) < 0)
			ft_error_message("error from dup2 in process_heredoc!");
		close (pipex->filein);
	}
}


int	main(int ac, char **av, char **envp)
{
	t_bpipex	*pipex;

	pipex = malloc(sizeof(t_bpipex));
	if (!pipex)
		ft_error_message("error from malloc!");
	if (ac < 5)
		ft_error_message("error from ac!");
	process_here_doc(ac, av, pipex);
	while (pipex->i < ac - 2)
		first_middle_cmds(av[pipex->i++], envp, pipex);
	last_cmd(ac, av, envp, pipex);
	close (pipex->fileout);
	close (0);
	free(pipex);
	while (wait(NULL) != -1)
		;
}