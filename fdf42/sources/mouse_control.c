/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:26:20 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/05 18:51:40 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int		ft_mouse_release(int button, int x, int y, t_mlx *mlx)
{
	y = button && x && y;
	mlx->mouse->isdown = FALSE;
	return (0);
}

int		ft_mouse_press(int button, int x, int y, t_mlx *mlx)
{
	y = button && x && y;
	mlx->mouse->isdown = TRUE;
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
		ft_render(mlx);
	}
	return (0);
}
