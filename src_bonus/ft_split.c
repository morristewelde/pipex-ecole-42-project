/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:43:21 by mtewelde          #+#    #+#             */
/*   Updated: 2024/11/21 22:50:50 by mtewelde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

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
