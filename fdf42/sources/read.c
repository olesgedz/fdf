/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:20:01 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/06 15:24:35 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "fdf.h"
#include <limits.h>

static int				ft_cleanup(t_list **lst, t_map **map)
{
	t_list	*next;

	while (*lst)
	{
		next = (*lst)->next;
		ft_memdel(&(*lst)->content);
		ft_memdel((void **)lst);
		*lst = next;
	}
	if (map && *map)
	{
		ft_memdel((void **)&(*map)->vectors);
		ft_memdel((void **)map);
	}
	return (0);
}

/*
**void					ft_printMap(t_map *map)
**{
**	int j = 0;
**	int k = 0;
**	while (j < map->height)
**	{
**		while (k < map->width)
**		{
**			if (0 <= abs(ft_vector_at(map, k, j).z)
** && abs(ft_vector_at(map, k, j).z) <= 9)
**				printf("%0.f  ", ft_vector_at(map, k, j).z);
**			else
**				printf("%0.f ", ft_vector_at(map, k, j).z);
**			k++;
**		}
**		printf("\n");
**		k = 0;
**		j++;
**	}
**}
*/

static void				ft_find_depth(t_map *m)
{
	int			min;
	int			max;
	t_vector	v;
	t_vector	cur;

	min = INT_MAX;
	max = INT_MIN;
	v.y = 0;
	while (v.y < m->height)
	{
		v.x = 0;
		while (v.x < m->width)
		{
			cur = *m->vectors[(int)v.y * m->width + (int)v.x];
			if (cur.z < min)
				min = cur.z;
			if (cur.z > max)
				max = cur.z;
			v.x++;
		}
		v.y++;
	}
	m->depth_min = min;
	m->depth_max = max;
}

static int				ft_populate_map(t_map **m, t_list *list)
{
	t_list	*lst;
	char	**split;
	int		x;
	int		y;

	lst = list;
	y = -1;
	while (++y < (*m)->height)
	{
		x = 0;
		if ((split = ft_strsplit(lst->content, ' ')) == NULL)
			return (ft_cleanup(&list, m));
		while (x < (*m)->width)
		{
			(*m)->vectors[y * (*m)->width + x] =
				ft_get_vector(x, y, ft_atoi(split[x]));
			x++;
		}
		ft_2darrayclean(&split);
		lst = lst->next;
	}
	ft_find_depth(*m);
	ft_fill_colors(*m);
	ft_cleanup(&list, NULL);
	return (1);
}

static int				ft_get_lines(int fd, t_list **lst)
{
	t_list	*temp;
	int		expected;
	char	*line;
	int		ret;

	expected = -1;
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		if (expected == -1)
			expected = (int)ft_countwords(line, ' ');
		temp = ft_lstnew(line, ft_strlen(line) + 1);
		if ((temp) == NULL || !ft_check_line(line))
			return (ft_cleanup(lst, NULL));
		ft_lstadd(lst, temp);
		if (expected != (int)ft_countwords(line, ' '))
			return (ft_cleanup(lst, NULL));
		ft_strdel(&line);
	}
	if (expected == -1 || ret == -1)
		return (ft_cleanup(lst, NULL));
	ft_lstrev(lst);
	return (1);
}

int						ft_read_file(int fd, t_map **m)
{
	t_list	*lst;

	lst = NULL;
	if (!(ft_get_lines(fd, &lst)))
		return (0);
	*m = get_map(ft_countwords(lst->content, ' '), ft_lstcount(lst));
	if (*m == NULL)
		return (ft_cleanup(&lst, m));
	return (ft_populate_map(m, lst));
}
