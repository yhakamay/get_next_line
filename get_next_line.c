/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhakamaya <yhakamaya@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 16:02:30 by yhakamay          #+#    #+#             */
/*   Updated: 2021/01/24 09:22:11 by yhakamaya        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_return(char *save)
{
	free(save);
	save = NULL;
	return (NULL);
}

static int	count_till_endl(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != '\n')
		i++;
	return (i);
}

static char	*gnl_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	total_len;
	char	*ret;

	if (!s1 && !s2)
		return (NULL);
	len1 = ft_strlen((char *)s1);
	len2 = ft_strlen((char *)s2);
	total_len = len1 + len2;
	if (!(ret = (char *)malloc((total_len + 1) * sizeof(char))))
		return (NULL);
	ft_memmove(ret, s1, len1);
	ft_memmove(ret + len1, s2, len2);
	ret[total_len] = '\0';
	free((char *)s1);
	return (ret);
}

static char	*get_save(char *save)
{
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!save)
		return (NULL);
	while (save[i] != '\n')
	{
		if (!save[i])
			return (free_return(save));
		i++;
	}
	if (!(ret = malloc(sizeof(char) * ((ft_strlen(save) - i) + 1))))
		return (NULL);
	i++;
	while (save[i])
		ret[j++] = save[i++];
	ret[j] = '\0';
	free(save);
	return (ret);
}

int			get_next_line(int fd, char **line)
{
	int			read_res;
	char		*buf;
	static char	*save;
	read_res = SUCCESS;
	//if (fd < 0 || !line || BUFFER_SIZE < 1 ||
	//	!(buf = (char *)malloc((size_t)BUFFER_SIZE + 1)))
	//	return (READ_ERROR);
	// malloc()が先にあるとマズい；fdなど他の項目で弾かれたのに、mallocは既にされてしまっている
	if (fd < 0 || !line || BUFFER_SIZE < 1 ||
		!(buf = (char *)malloc((size_t)BUFFER_SIZE + 1)))
		return (READ_ERROR);
	while (!(ft_strchr(save, '\n')) && read_res != 0)
	{
		if ((read_res = read(fd, buf, BUFFER_SIZE)) == READ_ERROR)
		{
			free(buf);
			return (READ_ERROR);
		}
		buf[read_res] = '\0';
		save = gnl_strjoin(save, buf);
	}
	free(buf);
	*line = ft_substr(save, 0, count_till_endl(save));
	save = get_save(save);
	return ((read_res == END_OF_FILE) ? END_OF_FILE : SUCCESS);
}
