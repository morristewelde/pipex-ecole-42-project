/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:30:58 by mtewelde          #+#    #+#             */
/*   Updated: 2024/12/02 21:32:19 by mtewelde         ###   ########.fr       */
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

char	*get_next_line(int fd)
{
	static char	read_buffer[BUFFER_SIZE + 1] = "\0";
	char		*read_line;
	char		*temp_line;
	int			bytes_read;

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
