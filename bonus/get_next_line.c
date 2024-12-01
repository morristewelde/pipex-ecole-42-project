/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:30:58 by mtewelde          #+#    #+#             */
/*   Updated: 2024/12/01 20:55:56 by mtewelde         ###   ########.fr       */
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
	char		*temp_line;
	int			bytes_read;

	read_line = NULL;
	read_line = ft_strdup(read_buffer);
	if (!read_line)
		return (NULL);
	while (!ft_check_line(read_line))
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			return (free(read_line), NULL);
		read_buffer[bytes_read] = '\0';
		temp_line = ft_strjoin(read_line, read_buffer);
		free(read_line);
		if (!temp_line)
			return (NULL);
		read_line = temp_line;
	}
	ft_set_next_line(read_buffer);
	return (read_line);
}
