/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:30:58 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/30 03:01:26 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

void	ft_bzero(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != 0)
	{
		str[i] = 0;
		i++;
	}
}

char	*ft_strdup(char *str)
{
	size_t	i;
	size_t	len;
	char	*res;

	len = 0;
	while (str[len] != '\n' && str[len] != 0)
		len++;
	if (str[len] == '\n')
		len++;
	res = malloc(sizeof(char) * (len + 1));
	if (res == NULL)
		return (res);
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = 0;
	return (res);
}

int	ft_check_line(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	ft_set_next_line(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i] != '\n' && str[i] != 0)
		i++;
	if (str[i] == '\n')
		i++;
	j = 0;
	while (str[i + j])
	{
		str[j] = str[i + j];
		j++;
	}
	str[j] = 0;
}

char	*ft_extract(int fd, char *line, char *buff)
{
	int			ft_fd;

	ft_fd = 1;
	while (ft_fd && ft_check_line(line) == 0)
	{
		ft_fd = read(fd, buff, BUFFER_SIZE);
		if (ft_fd < 0)
			return (ft_bzero(buff), free(line), NULL);
		buff[ft_fd] = 0;
		line = ft_strjoin(line, buff);
		if (line == NULL)
			return (NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	read_buffer[BUFFER_SIZE + 1] = "\0";
	char		*read_line;

	read_line = ft_strdup(read_buffer);
	if (read_line == NULL)
		return (NULL);
	read_line = ft_extract(fd, read_line, read_buffer);
	if (read_line == NULL)
		return (free(read_line), NULL);
	ft_set_next_line(read_buffer);
	if (read_line[0] == 0)
		return (free(read_line), NULL);
	return (read_line);
}
