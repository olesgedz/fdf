/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 17:14:14 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/04 20:34:04 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minilibx_macos/mlx.h"
#include <stdio.h>
#include <fcntl.h>
#include "libft/includes/libft.h"
#include <limits.h>
#include <math.h>

#define WIN_WIDTH			1680
#define WIN_HEIGHT			720
#define MENU_WIDTH		400
#define W_KEY 13
#define A_KEY 0
#define S_KEY 1
#define D_KEY 2
#define U_KEY 32
#define I_KEY 34
#define O_KEY 31
#define J_KEY 38
#define K_KEY 40
#define L_KEY 37
#define M_KEY 46
#define L_AR_KEY 43
#define TRUE		1
#define FALSE		0
#define ABS(x)  ( (x < 0) ? -(x) : x )

typedef struct		s_cam
{
	double		offsetx;
	double		offsety;
	double		x;
	double		y;
	double		z;
	int			scale;
	double		**matrix;
}					t_cam;
typedef struct		s_vector
{
	double		x;
	double		y;
	double		z;
	int			color;
}					t_vector;
typedef struct		s_map
{
	int			width;
	int			height;
	int			depth_min;
	int			depth_max;
	t_vector	**vectors;
}					t_map;
typedef struct		s_mouse
{
	char		isdown;
	int			x;
	int			y;
	int			lastx;
	int			lasty;
}					t_mouse;
typedef struct		s_image
{
	void		*image;
	char		*ptr;
	int			bpp;
	int			stride;
	int			endian;
}					t_image;
typedef struct s_keyboard
{
	int w;
	int a;
	int s;
	int d;
	int u;
	int i;
	int o;
	int j;
	int k;
	int l;
	int m;
	int l_ar;
} t_keyboard;
typedef struct		s_mlx
{
	void		*mlx;
	void		*window;
	t_image		*image;
	t_map		*map;
	t_cam		*cam;
	t_mouse		*mouse;
	t_keyboard *keyboard;
	double		**zbuf;
}					t_mlx;
typedef struct		s_line
{
	t_vector	start;
	t_vector	end;
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	int			err;
	int			err2;
}					t_line;

int		ft_error(char *reason)
{
	ft_putendl(reason);
	return (1);
}

int		get_light(int start, int end, double percentage)
{
	return ((int)((1 - percentage) * start + percentage * end));
}

double		percent(int start, int end, int current)
{
	double placement;
	double distance;

	placement = current - start;
	distance = end - start;
	return ((distance == 0) ? 1.0 : (placement / distance));
}

int			ft_get_color(int c1, int c2, double p)
{
	int r;
	int g;
	int b;

	if (c1 == c2)
		return (c1);
	r = get_light((c1 >> 16) & 0xFF, (c2 >> 16) & 0xFF, p);
	g = get_light((c1 >> 8) & 0xFF, (c2 >> 8) & 0xFF, p);
	b = get_light(c1 & 0xFF, c2 & 0xFF, p);
	return (r << 16 | g << 8 | b);
}

size_t	ft_lstcount(t_list *lst)
{
	size_t i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void		ft_lstrev(t_list **alst)
{
	t_list	*prev;
	t_list	*cur;
	t_list	*next;

	prev = NULL;
	cur = *alst;
	while (cur != NULL)
	{
		next = cur->next;
		cur->next = prev;
		prev = cur;
		cur = next;
	}
	*alst = prev;
}

int		ft_is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t'
		|| c == '\v' || c == '\r' || c == '\f' || c == '\0')
		return (1);
	return (0);
}

size_t	ft_countwords(char *str, char sep)
{
	size_t result;
	size_t i;

	i = 0;
	result = 0;
	while (str[i] && str[i] == sep)
		i++;
	while (str[i])
	{
		while (str[i] && str[i] != sep)
			i++;
		result++;
		while (str[i] && str[i] == sep)
			i++;
	}
	return (result);
}

static int	cleanup(t_list **lst, t_map **map)
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

t_map		*get_map(int width, int height)
{
	t_map	*map;

	map = ft_memalloc(sizeof(t_map));
	if (map == NULL)
		return (NULL);
	map->width = width;
	map->height = height;
	map->depth_min = 0;
	map->depth_max = 0;
	map->vectors = ft_memalloc(sizeof(t_vector *) * width * height);
	if (map->vectors == NULL)
	{
		ft_memdel((void **)&map);
		return (NULL);
	}
	return (map);
}

static int	get_lines(int fd, t_list **lst)
{
	t_list	*temp;
	int		expected;
	char	*line;
	int		ret;

	expected = -1;
	while ((ret = get_next_line(fd, &line)))
	{
		if (expected == -1)
			expected = (int)ft_countwords(line, ' ');
		temp = ft_lstnew(line, ft_strlen(line) + 1);
		if ((temp) == NULL)
			return (cleanup(lst, NULL));
		ft_lstadd(lst, temp);
		if (expected != (int)ft_countwords(line, ' '))
			return (cleanup(lst, NULL));
		ft_strdel(&line);
	}
	if (expected == -1 || ret == -1)
		return (cleanup(lst, NULL));
	ft_lstrev(lst);
	return (1);
}

t_vector	*get_vector(int x, int y, char *str)
{
	t_vector	*v;

	v = ft_memalloc(sizeof(t_vector));
	if (v == NULL)
		return (NULL);
	v->x = (double)x;
	v->y = (double)y;
	v->z = (double)ft_atoi(str);
	v->color = 0xFFFFFF;
	return (v);
}

void		find_depth(t_map *m)
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

t_vector	vector_at(t_map *map, int x, int y)
{
	return (*map->vectors[y * map->width + x]);
}

void		fill_colors(t_map *m)
{
	t_vector	v;
	t_vector	*cur;

	v.y = 0;
	while (v.y < m->height)
	{
		v.x = 0;
		while (v.x < m->width)
		{
			cur = m->vectors[(int)v.y * m->width + (int)v.x];
			cur->color = ft_get_color(0xFF0000, 0xFFFFFF, (cur->z -
				m->depth_min) / (m->depth_max - m->depth_min));
			v.x++;
		}
		v.y++;
	}
}

static int	populate_map(t_map **m, t_list *list)
{
	t_list	*lst;
	char	**split;
	int		x;
	int		y;

	lst = list;
	y = 0;
	while (y < (*m)->height)
	{
		x = 0;
		if ((split = ft_strsplit(lst->content, ' ')) == NULL)
			return (cleanup(&list, m));
		while (x < (*m)->width)
		{
			(*m)->vectors[y * (*m)->width + x] = get_vector(x, y, split[x]);
			x++;
		}
		ft_2darrayclean(&split);
		lst = lst->next;
		y++;
	}
	find_depth(*m);
	fill_colors(*m);
	cleanup(&list, NULL);
	return (1);
}

int			read_file(int fd, t_map **m)
{
	t_list	*lst;

	lst = NULL;
	if (!(get_lines(fd, &lst)))
		return (0);
	*m = get_map(ft_countwords(lst->content, ' '), ft_lstcount(lst));
	if (*m == NULL)
		return (cleanup(&lst, m));
	return (populate_map(m, lst));
}

/*
**void		ft_printMap(t_map *map)
**{
**	int j = 0;
**	int k = 0;
**	while (j < map->height)
**	{
**		while (k < map->width)
**		{
**			if (0 <= abs(vector_at(map, k, j).z)
** && abs(vector_at(map, k, j).z) <= 9)
**				printf("%0.f  ", vector_at(map, k, j).z);
**			else
**				printf("%0.f ", vector_at(map, k, j).z);
**			k++;
**		}
**		printf("\n");
**		k = 0;
**		j++;
**	}
**}
*/

t_image	*del_image(t_mlx *mlx, t_image *img)
{
	if (img != NULL)
	{
		if (img->image != NULL)
			mlx_destroy_image(mlx->mlx, img->image);
		ft_memdel((void **)&img);
	}
	return (NULL);
}

t_mlx		*mlxdel(t_mlx *mlx)
{
	if (mlx->window != NULL)
		mlx_destroy_window(mlx->mlx, mlx->window);
	if (mlx->cam != NULL)
		ft_memdel((void **)&mlx->cam);
	if (mlx->mouse != NULL)
		ft_memdel((void **)&mlx->mouse);
	if (mlx->image != NULL)
		del_image(mlx, mlx->image);
	ft_memdel((void **)&mlx);
	return (NULL);
}

void	clear_image(t_image *image)
{
	ft_bzero(image->ptr, WIN_WIDTH * WIN_HEIGHT * image->bpp);
}

t_image	*new_image(t_mlx *mlx)
{
	t_image		*img;

	if ((img = ft_memalloc(sizeof(t_image))) == NULL)
		return (NULL);
	if ((img->image = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT)) == NULL)
		return (del_image(mlx, img));
	img->ptr = mlx_get_data_addr(img->image, &img->bpp, &img->stride,
			&img->endian);
	img->bpp /= 8;
	return (img);
}

t_mlx		*init(char *title, t_map *map)
{
	t_mlx	*mlx;

	if ((mlx = ft_memalloc(sizeof(t_mlx))) == NULL)
		return (NULL);
	if ((mlx->mlx = mlx_init()) == NULL ||
		(mlx->window = mlx_new_window(mlx->mlx, WIN_WIDTH,
			WIN_HEIGHT, title)) == NULL ||
		(mlx->cam = ft_memalloc(sizeof(t_cam))) == NULL ||
		(mlx->mouse = ft_memalloc(sizeof(t_mouse))) == NULL ||
		(mlx->keyboard = ft_memalloc(sizeof(t_mouse))) == NULL ||
		(mlx->image = new_image(mlx)) == NULL)
		return (mlxdel(mlx));
	mlx->cam->x = -M_PI / 6;
	mlx->cam->y = -M_PI / 6;
	mlx->cam->z = 0;
	mlx->cam->scale = map->width < 50 ? 32 : 5;
	mlx->cam->offsetx = (WIN_WIDTH - MENU_WIDTH) / 2;
	mlx->cam->offsety = WIN_HEIGHT / 2;
	return (mlx);
}

void	image_set_pixel(t_image *image, int x, int y, int color)
{
	if (x < 0 || x >= WIN_WIDTH - MENU_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	*(int *)(image->ptr + ((x + y * WIN_WIDTH) * image->bpp)) = color;
}

t_vector	rotate(t_vector p, t_cam *r)
{
	t_vector	v;
	double		x;
	double		y;
	double		z;

	x = p.x;
	z = p.z;
	v.x = cos(r->y) * x + sin(r->y) * z;
	v.z = -sin(r->y) * x + cos(r->y) * z;
	y = p.y;
	z = v.z;
	v.y = cos(r->x) * y - sin(r->x) * z;
	v.z = sin(r->x) * y + cos(r->x) * z;
	x = v.x;
	y = v.y;
	v.x = cos(r->z) * x - sin(r->z) * y;
	v.y = sin(r->z) * x + cos(r->z) * y;
	v.color = p.color;
	return (v);
}

t_vector	project_vector(t_vector v, t_mlx *mlx)
{
	v.x -= (double)(mlx->map->width - 1) / 2.0f;
	v.y -= (double)(mlx->map->height - 1) / 2.0f;
	v.z -= (double)(mlx->map->depth_min + mlx->map->depth_max) / 2.0f;
	v = rotate(v, mlx->cam);
	v.x *= mlx->cam->scale;
	v.y *= mlx->cam->scale;
	v.x += mlx->cam->offsetx;
	v.y += mlx->cam->offsety;
	return (v);
}

int		ft_sign(double n)
{
	return (n < 0 ? -1 : 1);
}

int		ft_put_points(t_mlx *mlx, t_line *l, t_vector *p1, t_vector *p2)
{
	double percentage;

	if (l->dx > l->dy)
		percentage = percent(l->start.x, l->end.x, p1->x);
	else
		percentage = percent(l->start.y, l->end.y, p1->y);
	image_set_pixel(mlx->image, (int)p1->x,
	(int)p1->y, ft_get_color(p1->color, p2->color, percentage));
	l->err2 = l->err;
	if (l->err2 > -l->dx)
	{
		l->err -= l->dy;
		p1->x += l->sx;
	}
	if (l->err2 < l->dy)
	{
		l->err += l->dx;
		p1->y += l->sy;
	}
	return (0);
}

void		ft_plotline(t_mlx *mlx, t_vector p1, t_vector p2)
{
	t_line	line;

	p1.x = (int)p1.x;
	p2.x = (int)p2.x;
	p1.y = (int)p1.y;
	p2.y = (int)p2.y;
	line.start = p1;
	line.end = p2;
	line.dx = (int)ABS((int)p2.x - (int)p1.x);
	line.sx = (int)p1.x < (int)p2.x ? 1 : -1;
	line.dy = (int)ABS((int)p2.y - (int)p1.y);
	line.sy = (int)p1.y < (int)p2.y ? 1 : -1;
	line.err = (line.dx > line.dy ? line.dx : -line.dy) / 2;
	while (((int)p1.x != (int)p2.x || (int)p1.y != (int)p2.y))
		if (ft_put_points(mlx, &line, &p1, &p2))
			break ;
}

void		ft_draw_menu(t_mlx *mlx)
{
	int		y;

	y = 0;
	mlx_string_put(mlx->mlx, mlx->window, WIN_WIDTH - MENU_WIDTH, 0, 0xFFFFFFF, "How to Use");
}


static void	ft_draw_background(t_mlx *mlx)
{
	t_image *image;
	int	i;

	clear_image(mlx->image);
	image = mlx->image;
	int j  = 0;
	int k = 0;
	while ( j < WIN_HEIGHT)
	{
		k = 0;
		while ( k < WIN_WIDTH)
		{
			if (k  < WIN_WIDTH - MENU_WIDTH)
				*(int *)(image->ptr + ((k + j * WIN_WIDTH) * image->bpp)) = 0x1E1E1E;
			else
				*(int *)(image->ptr + ((k + j * WIN_WIDTH) * image->bpp)) = 0x222222;
			k++;
		}
		j++;
	}
}


void		render(t_mlx *mlx)
{
	int			x;
	int			y;
	t_vector	v;
	t_map		*map;

	map = mlx->map;
	x = 0;
	ft_draw_background(mlx);
	ft_draw_menu(mlx);
	while (x < map->width)
	{
		y = 0;
		while (y < map->height)
		{
			v = project_vector(vector_at(map, x, y), mlx);
			if (x + 1 < map->width)
				ft_plotline(mlx, v,
					project_vector(vector_at(map, x + 1, y), mlx));
			if (y + 1 < map->height)
			ft_plotline(mlx, v,
				project_vector(vector_at(map, x, y + 1), mlx));
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(mlx->mlx, mlx->window, mlx->image->image, 0, 0);
	ft_draw_menu(mlx);
}

void		ft_press_move(t_mlx *mlx)
{
	if (mlx->keyboard->d == TRUE)
		mlx->cam->offsetx += 20;
	if (mlx->keyboard->a == TRUE)
		mlx->cam->offsetx -= 20;
	if (mlx->keyboard->s == TRUE)
		mlx->cam->offsety += 20;
	if (mlx->keyboard->w == TRUE)
		mlx->cam->offsety -= 20;
	if (mlx->keyboard->u == TRUE)
		mlx->cam->scale -= 0.5;
	if (mlx->keyboard->o == TRUE)
		mlx->cam->scale += 1.5;
	if (mlx->keyboard->m == TRUE)
		mlx->cam->z -= 0.1;
	if (mlx->keyboard->l_ar == TRUE)
		mlx->cam->z += 0.1;
	if (mlx->keyboard->i == TRUE)
		mlx->cam->x += 0.1;
	if (mlx->keyboard->j == TRUE)
		mlx->cam->y -= 0.1;
	if (mlx->keyboard->k == TRUE)
		mlx->cam->x -= 0.1;
	if (mlx->keyboard->l == TRUE)
		mlx->cam->y += 0.1;
}

int		ft_handle_keys_press(int key, t_mlx *mlx)
{
	if (key == W_KEY)
		mlx->keyboard->w = TRUE;
	if (key == A_KEY)
		mlx->keyboard->a = TRUE;
	if (key == S_KEY)
		mlx->keyboard->s = TRUE;
	if (key == D_KEY)
		mlx->keyboard->d = TRUE;
	if (key == U_KEY)
		mlx->keyboard->u = TRUE;
	if (key == I_KEY)
		mlx->keyboard->i = TRUE;
	if (key == O_KEY)
		mlx->keyboard->o = TRUE;
	if (key == J_KEY)
		mlx->keyboard->j = TRUE;
	if (key == K_KEY)
		mlx->keyboard->k = TRUE;
	if (key == L_KEY)
		mlx->keyboard->l = TRUE;
	if (key == M_KEY)
		mlx->keyboard->m = TRUE;
	if (key == L_AR_KEY)
		mlx->keyboard->l_ar = TRUE;
	ft_press_move(mlx);
	render(mlx);
	return (0);
}

int		ft_handle_keys_release(int key, t_mlx *mlx)
{
	if (key == W_KEY)
		mlx->keyboard->w = FALSE;
	if (key == A_KEY)
		mlx->keyboard->a = FALSE;
	if (key == S_KEY)
		mlx->keyboard->s = FALSE;
	if (key == D_KEY)
		mlx->keyboard->d = FALSE;
	if (key == U_KEY)
		mlx->keyboard->u = FALSE;
	if (key == I_KEY)
		mlx->keyboard->i = FALSE;
	if (key == O_KEY)
		mlx->keyboard->o = FALSE;
	if (key == J_KEY)
		mlx->keyboard->j = FALSE;
	if (key == K_KEY)
		mlx->keyboard->k = FALSE;
	if (key == L_KEY)
		mlx->keyboard->l = FALSE;
	if (key == M_KEY)
		mlx->keyboard->m = FALSE;
	if (key == L_AR_KEY)
		mlx->keyboard->l_ar = FALSE;
	render(mlx);
	return (0);
}

int		ft_mouse_press(int button, int x, int y, t_mlx *mlx)
{
	mlx->mouse->isdown = TRUE;
	return (0);
}

int		ft_mouse_release(int button, int x, int y, t_mlx *mlx)
{
	mlx->mouse->isdown = FALSE;
	return (0);
}

int		ft_mouse_move(int x, int y, t_mlx *mlx)
{
	mlx->mouse->lastx = mlx->mouse->x;
	mlx->mouse->lasty = mlx->mouse->y;
	mlx->mouse->x = x;
	mlx->mouse->y = y;
	if (mlx->mouse->isdown == TRUE)
	{
		mlx->cam->y += (x - mlx->mouse->lastx) * 0.005;
		mlx->cam->x += -(y - mlx->mouse->lasty) * 0.005;
		render(mlx);
	}
	return (0);
}

int		ft_mlx_hooks(t_mlx *mlx)
{
	mlx_hook(mlx->window, 2, 0, ft_handle_keys_press, (void *)mlx);
	mlx_hook(mlx->window, 3, 0, ft_handle_keys_release, (void *)mlx);
	mlx_mouse_hook(mlx->window, ft_mouse_press, (void *)mlx);
	mlx_hook(mlx->window, 5, 0, ft_mouse_release, (void *)mlx);
	mlx_hook(mlx->window, 6, 0, ft_mouse_move, (void *)mlx);
	return (0);
}

int		main(int argc, char **argv)
{
	t_map	*map;
	t_mlx	*mlx;
	int		fd;

	fd = open(argv[1], O_RDONLY);
	if (argc != 2)
		ft_putstr("usage:./fdf map.fdf\n");
	if (fd < 0 || !read_file(fd, &map))
		return (ft_error("error: invalid file"));
	if ((mlx = init(ft_strjoin("FdF - ", argv[1]), map)) == NULL)
		return (ft_error("error: mlx couldn't init"));
	mlx->map = map;
	render(mlx);
	ft_mlx_hooks(mlx);
	mlx_loop(mlx->mlx);
	return (0);
}
