#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mlx.h>
#include <math.h>
#include "libft.h"
 #include <fcntl.h>
 #include <unistd.h>

typedef struct    data_s
{
		void          *mlx_ptr;
		void          *win_ptr;
		void					*img_ptr;
		char 					*img_data;
		int						w_width;
		int 					w_height;
		int 					bits_per_pixel;
		int 					size_line;
		int endian;
}                 data_t;

typedef struct point_s
{
	int x;
	int y;
	int z;
	int x_iso;
	int y_iso;
	int size;
} t_point3d;

typedef struct map_s
{
	size_t width;
	size_t height;
	t_point3d *points;
	int scale;
} t_map;

data_t        data;
t_point3d		point;
t_map map;

static void	ft_img_pixel_put(int x, int y, int color)
{
	int		i;

	if ( x < 500 && y < 500)
	{
		i = (x * data.bits_per_pixel / 8) + (y * data.size_line);
		data.img_data[i] = color;
		data.img_data[++i] = color >> 8;
		data.img_data[++i] = color >> 16;
	}
}

void ft_turnX(t_point3d *point, double degree)
{
	point->x = point->x;
	point->y = point->y * cos(degree) + point->z * sin(degree) ;
	point->z = -point->y * sin(degree) + point->z * cos(degree) ;
}

void ft_turnY(t_point3d *point, double degree)
{
	point->x = point->x * cos(degree) + point->z * sin(degree);
	point->y = point->y;
	point->z = -point->x * sin(degree) + point->z * cos(degree) ;
}

void ft_turnZ(t_point3d *point, double degree)
{
	point->x = point->x * cos(degree) -  point->y * sin(degree) ;
	point->y = -point->x * sin(degree) + point->y * cos(degree) ;
	point->z = point->z;
}


static void	iso(t_point3d *point)
{
	int previous_x;
	int previous_y;
		//printf("x:%d , y:%d, z:%d\n", point->x, point->y, point->z);
	previous_x = point->x;
	previous_y = point->y;
	point->x_iso = (previous_x - previous_y) * cos(0.523599);
	point->y_iso = -point->z + (previous_x + previous_y) * sin(0.523599);
	//printf("ISO:x:%d , y:%d, z:%d\n", point->x_iso, point->y_iso, point->z);
}


// void ft_draw()
// {
// 	int x = 0;
// 	int y = 0;
// 	data.img_ptr = mlx_new_image (data.mlx_ptr, data.w_width, data.w_height);
// 	data.img_data = mlx_get_data_addr (data.img_ptr, &data.bits_per_pixel, &data.size_line, &data.endian);
// 	int size = 1;
// 	iso(&point.x, &point.y, point.z);
// 	while (y < size)
// 	{
// 		 x = 0;
// 		while (x < size)
// 		{
// 			//mlx_pixel_put(data.mlx_ptr, data.win_ptr, k+y, j+x, 0xFFFFFF);
// 			ft_img_pixel_put( point.x + x, point.y + y,  0xFFFFFFF);
// 			x++;
// 		}
// 		y++;
// 	}
// 	mlx_put_image_to_window ( data.mlx_ptr, data.win_ptr, data.img_ptr, 0, 0);
// }

void ft_draw(t_point3d *point)
{
	//printf("x:%d y:%d z:%d x_iso:%d, y_iso:%d\n", point->x, point->y, point->z,point->y_iso, point->y_iso);
	iso(point);
	int color = 0xFFFFFFF;
	if (point->z != 0)
	 color = 0xff0000;
	ft_img_pixel_put(point->x_iso * map.scale, point->y_iso * map.scale,  color);

}

void ft_handle_keys(t_point3d *point, int key)
{
	if (key == 2)
		point->x += 20;
	if (key == 0)
		point->x -= 20;
	if (key == 13)
		point->y -= 20;
	if (key == 1)
		point->y += 20;
	if (key == 38)
		ft_turnX(point, M_PI / 12);
	if (key == 37)
		ft_turnY(point, M_PI / 12);
	if (key == 40)
		ft_turnZ(point, M_PI / 12);

}
int ft_main(int key, void *param)
{	// void new = mlx_new_image(data.mlx_ptr, 500 , 500);
	//
	// mlx_put_image_to_window (data.mlx_ptr, data.win_ptr, 0, 0);

	int x = 1;
	int y = 1;
	if (key == 24)
		map.scale += 1;
		if (key == 27)
			map.scale -= 1;
	
	printf("key: %d scale %d\n", key, map.scale);
	int i = 0;
	while (i < map.width * map.height)
	{
		ft_handle_keys(&map.points[i++], key);
	}
	//ft_handle_keys(&point, key);

	i = 0;
			//printf("x:%d y:%d z:%d key%d\n", point.x, point.y, point.z, key);
			data.img_ptr = mlx_new_image (data.mlx_ptr, data.w_width, data.w_height);
			data.img_data = mlx_get_data_addr (data.img_ptr, &data.bits_per_pixel, &data.size_line, &data.endian);
		int c = 0;
		while (i < map.width * map.height)
		{
			ft_draw(&map.points[i++]);
		}
		printf("END\n\n");
		mlx_put_image_to_window ( data.mlx_ptr, data.win_ptr, data.img_ptr, 0, 0);
	return (0);
	return (0);
}


int ft_is_space(char c)
{
	if (c == ' ' || c == '\n' || c == '\t'
		|| c == '\v' || c == '\r' || c == '\f' || c == '\0')
		return (1);
	return (0);
}

int ft_countWords(char *s)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while(ft_is_space(s[i]) && s[i])
			i++;
		if (!ft_is_space(s[i]))
			count++;
		while(!ft_is_space(s[i]) && s[i])
			i++;
	}
	return (count);
}

void ft_validateMap(char **argv)
{
	int fd = open(argv[1], O_RDONLY);
	int i = 0;
	char* line;
	while (get_next_line(fd, &line) > 0)
	{
		map.width = ft_countWords(line);
		free(line);
		i++;
	}
	close(fd);
	map.height = i;
}

void ft_save_points(char **argv)
{
	int fd = open(argv[1], O_RDONLY);
	int i = 0;
	char *line;
	char **temp;
	int j = 0;
	int nPoint = 0;

	map.points = malloc(sizeof(t_point3d) * map.width * map.height);
	while (get_next_line(fd, &line) > 0)
	{
		temp = ft_strsplit(line, ' ');
		j = 0;
		while (temp[j] != NULL)
		{
			map.points[nPoint].x = j;
			map.points[nPoint].y = i;
			map.points[nPoint].z = ft_atoi(temp[j]);
			//printf("%f %d ", map.points[nPoint].z, nPoint);
			//printf("%d\n", ft_atoi(temp[j]));
			nPoint++;
			j++;
		}
		//free temp
		free(line);
		i++;
	}
	close(fd);
}

static void		ft_printMap()
{
	int i = 0;
	int count = 0;
	while (i < map.width * map.height)
	{
		printf("%d ", map.points[i].z);
		count++;
		if (count == map.width)
		{
			count = 0;
			printf("\n");
		}

		i++;
	}
}

int main(int argc, char **argv)
{
		map.height = 0;
		map.width = 0;
	    data.w_height = 500;
			data.w_width = 500;
			data.bits_per_pixel  = 32;
			data.size_line = 7680;
			data.endian = 1;
				map.scale = 1;

			if (argc != 2)
			{
				ft_putstr("usage:\n");
				return (0);
			}
			point.x = -47;
			point.y = 48;
			point.z = 190;
			ft_validateMap(argv);
			ft_save_points(argv);
			ft_printMap();
		    if ((data.mlx_ptr = mlx_init()) == NULL)
				        return (EXIT_FAILURE);
			    if ((data.win_ptr = mlx_new_window(data.mlx_ptr, data.w_height, data.w_width, "Hello world")) == NULL)
					        return (EXIT_FAILURE);

						ft_main(3,(void *)0);
						printf("%d", mlx_key_hook(data.win_ptr, ft_main, (void *)0));
					//	mlx_string_put(data.mlx_ptr, data.win_ptr, j, k, 0xFFFFFF, "HELLO");
				    mlx_loop(data.mlx_ptr);
					    return (EXIT_SUCCESS);
				return (0);
}
