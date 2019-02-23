/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 17:14:14 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/05 17:07:56 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "fdf.h"
#include "mlx.h"
#include "libft.h"

int		main(int argc, char **argv)
{
	t_map	*map;
	t_mlx	*mlx;
	int		fd;

	fd = open(argv[1], O_RDONLY);
	if (argc != 2)
		return (ft_error("usage:./fdf map.fdf"));
	if (fd < 0 || !ft_read_file(fd, &map))
		return (ft_error("error: invalid file"));
	if ((mlx = ft_init(ft_strjoin("FdF - ", argv[1]), map)) == NULL)
		return (ft_error("error: mlx couldn't init"));
	mlx->map = map;
	ft_render(mlx);
	ft_mlx_hooks(mlx);
	mlx_loop(mlx->mlx);
	return (0);
}
