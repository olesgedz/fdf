/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:27:18 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/05 19:11:17 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "libft.h"
#include "mlx.h"

static int			ft_put_points(t_mlx *mlx,
		t_line *l, t_vector *p1, t_vector *p2)
{
	double percentage;

	if (l->dx > l->dy)
		percentage = ft_percent(l->start.x, l->end.x, p1->x);
	else
		percentage = ft_percent(l->start.y, l->end.y, p1->y);
	ft_image_set_pixel(mlx->image, (int)p1->x,
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

static void			ft_plotline(t_mlx *mlx, t_vector p1, t_vector p2)
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

static int			ft_draw_menu(t_mlx *mlx)
{
	int		y;

	y = 0;
	mlx_string_put(mlx->mlx, mlx->window,
		WIN_WIDTH - MENU_WIDTH + 10, y, 0xFFFFFFF, "How to Use");
	mlx_string_put(mlx->mlx, mlx->window,
		WIN_WIDTH - MENU_WIDTH + 10, y += 25, 0xFFFFFFF, "UO to zoom");
	mlx_string_put(mlx->mlx, mlx->window,
		WIN_WIDTH - MENU_WIDTH + 10, y += 25, 0xFFFFFFF, "WASD to shift");
	mlx_string_put(mlx->mlx, mlx->window,
		WIN_WIDTH - MENU_WIDTH + 10, y += 25, 0xFFFFFFF, "IJKL to rotate or ");
	mlx_string_put(mlx->mlx, mlx->window, WIN_WIDTH - MENU_WIDTH + 30,
			y += 25, 0xFFFFFFF, "press lmb and use mouse");
	mlx_string_put(mlx->mlx, mlx->window, WIN_WIDTH - MENU_WIDTH + 10,
			y += 25, 0xFFFFFFF, "M< to rotate on Z axis");
	mlx_string_put(mlx->mlx, mlx->window,
		WIN_WIDTH - MENU_WIDTH + 10, y += 25, 0xFFFFFFF, "B to change color");
	return (0);
}

static void			ft_draw_background(t_mlx *mlx)
{
	t_image *image;
	int		j;
	int		k;

	ft_clear_image(mlx->image);
	image = mlx->image;
	j = 0;
	while (j < WIN_HEIGHT)
	{
		k = -1;
		while (k++ < WIN_WIDTH)
			*(int *)(image->ptr + ((k + j * WIN_WIDTH) * image->bpp)) =
			k < WIN_WIDTH - MENU_WIDTH ? 0x1E1E1E : 0x222222;
		j++;
	}
}

void				ft_render(t_mlx *mlx)
{
	int			x;
	int			y;
	t_vector	v;
	t_map		*map;

	map = mlx->map;
	x = -1;
	ft_draw_background(mlx);
	while (++x < map->width)
	{
		y = 0;
		while (y < map->height)
		{
			v = ft_project_vector(ft_vector_at(map, x, y), mlx);
			if (x + 1 < map->width)
				ft_plotline(mlx, v,
					ft_project_vector(ft_vector_at(map, x + 1, y), mlx));
			if (y + 1 < map->height)
				ft_plotline(mlx, v,
					ft_project_vector(ft_vector_at(map, x, y + 1), mlx));
			y++;
		}
	}
	mlx_put_image_to_window(mlx->mlx, mlx->window, mlx->image->image, 0, 0);
	ft_draw_menu(mlx);
}
