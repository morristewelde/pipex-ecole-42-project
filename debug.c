
#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>

typedef struct s_pipex
{
	char	*path;
	int		fd1;
	int		fd2;
	char	**av;
	char	**envp;
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		filein;
	int		fileout;
	char	**split_cmd;
	char	**chec;
	char	*result;
}			t_pipex;

char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
char	**ft_get_path(char **env);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*check_cmd(char **env, char **cmd);
void	ft_error_message(char *mess);
void	ft_free_leak(char **str);
void	pipex(int ac, char **av, t_pipex *xpipe, char **envp);
size_t	ft_strlen(const char *str);
char	*ft_searsh_for_slash(const char *s, int c);

# define RED "\x1b[31m" 
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define RESET "\x1b[0m"

#endif

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

// -  -  -  -  split_cmd


char	*ft_strdup(const char *src)
{
	char	*dst;
	size_t	len;
	size_t	i;

	len = 0;
	while (*src)
		len++;
	dst = (char *) malloc(sizeof(char) * (len + 1));
	if (dst == NULL)
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
	if (str == NULL)
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

//  -  -  -  -  utils

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
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
		i++;
	}
	ft_free_leak(envp);
	return (NULL);
}

//  -  -  -  -  -  - pipex  -  -  -  -  -  -  //

void	ft_execve(char *cmd, char **envp, t_pipex *xpipe)
{
	xpipe->split_cmd = ft_split(cmd, ' ');
	xpipe->chec = ft_get_path(envp);
	if (!xpipe->split_cmd[0] || !xpipe->chec)
		ft_error_message(GREEN "command not found!" RESET);
	xpipe->result = check_cmd(xpipe->chec, xpipe->split_cmd);
	if (xpipe->result)
	{
		if (execve(xpipe->result, xpipe->split_cmd, envp) == -1)
			ft_error_message(CYAN "error message from execve!" RESET);
	}
	else
		ft_error_message(YELLOW "error from access!" RESET);
}

void	child_process1(char **av, t_pipex *xpipe, char **envp)
{
	xpipe->filein = open(av[1], O_RDONLY, 0644);
	if (xpipe->filein == -1)
	{
		(close(xpipe->fd[0]), close(xpipe->fd[1]));
		ft_error_message(MAGENTA "error message from filein!" RESET);
	}
	close(xpipe->fd[0]);
	if (dup2(xpipe->filein, STDIN_FILENO) < 0)
		ft_error_message("error from dup2 in child2!");
	close(xpipe->filein);
	if (dup2(xpipe->fd[1], STDOUT_FILENO) < 0)
		ft_error_message("error from dup2 in child2!");
	close(xpipe->fd[1]);
	ft_execve(av[2], envp, xpipe);
}

void	child_process2(char **av, t_pipex *xpipe, char **envp)
{
	xpipe->fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (xpipe->fileout == -1)
	{
		(close(xpipe->fd[0]), close(xpipe->fd[1]));
		ft_error_message(BLUE "error message from fileout!" RESET);
	}
	close(xpipe->fd[1]);
	if (dup2(xpipe->fd[0], STDIN_FILENO) < 0)
		ft_error_message("error from dup2 in child2!");
	close(xpipe->fd[0]);
	if (dup2(xpipe->fileout, STDOUT_FILENO) < 0)
		ft_error_message("error from dup2 in child2!");
	close(xpipe->fileout);
	ft_execve(av[3], envp, xpipe);
}

void	pipex(int ac, char **av, t_pipex *xpipe, char **envp)
{
	if (ac != 5)
		ft_error_message(RED "error message from ac!" RESET);
	if (!envp)
		ft_error_message(YELLOW "error message from envp!" RESET);
	if (pipe(xpipe->fd) < 0)
		ft_error_message(YELLOW "error message from pipe!" RESET);
	xpipe->pid1 = fork();
	if (xpipe->pid1 == -1)
		ft_error_message(BLUE "error message from pid1!" RESET);
	if (xpipe->pid1 == 0)
		child_process1(av, xpipe, envp);
	xpipe->pid2 = fork();
	if (xpipe->pid2 == -1)
		ft_error_message(RED "error message from pid2!" RESET);
	if (xpipe->pid2 == 0)
		child_process2(av, xpipe, envp);
}

//  -  -  -  -  -  -  -  main   -  -  -  -  -  - //

int	main(int ac, char **av, char **envp)
{
	t_pipex	*xpipe;

	xpipe = malloc(sizeof(t_pipex));
	if (!xpipe)
		ft_error_message(RED "error message from malloc!" RESET);
	pipex(ac, av, xpipe, envp);
	close(xpipe->fd[0]);
	close(xpipe->fd[1]);
	wait(NULL);
	wait(NULL);
	free (xpipe);
}