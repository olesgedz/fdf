#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mlx.h>
#include <math.h>

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
	double x;
	double y;
	double z;
	int size;
} t_point;

data_t        data;
t_point		point;

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

void ft_turnX(t_point *point, double degree)
{
	point->x = point->x;
	point->y = point->y * cos(degree) + point->z * sin(degree) ;
	point->z = -point->y * sin(degree) + point->z * cos(degree) ;
}

void ft_turnY(t_point *point, double degree)
{
	point->x = point->x * cos(degree) + point->z * sin(degree);
	point->y = point->y;
	point->z = -point->x * sin(degree) + point->z * cos(degree) ;
}

void ft_turnZ(t_point *point, double degree)
{
	point->x = point->x * cos(degree) -  point->y * sin(degree) ;
	point->y = -point->x * sin(degree) + point->y * cos(degree) ;
	point->z = point->z;
}


static void iso(double *x, double *y, double z)
{
    int previous_x;
    int previous_y;

    previous_x = *x;
    previous_y = *y;
    *x = (previous_x - previous_y) * cos(0.523599);
    *y = -z + (previous_x + previous_y) * sin(0.523599);
}


void ft_draw()
{
	int x = 0;
	int y = 0;
	data.img_ptr = mlx_new_image (data.mlx_ptr, data.w_width, data.w_height);
	data.img_data = mlx_get_data_addr (data.img_ptr, &data.bits_per_pixel, &data.size_line, &data.endian);
	int size = 1;
	//iso(&point.x, &point.y, point.z);
	while (y < size)
	{
		 x = 0;
		while (x < size)
		{
			//mlx_pixel_put(data.mlx_ptr, data.win_ptr, k+y, j+x, 0xFFFFFF);
			ft_img_pixel_put( point.x + x, point.y + y,  0xFFFFFFF);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window ( data.mlx_ptr, data.win_ptr, data.img_ptr, 0, 0);
}



int ft_main(int key, void *param)
{
	// void new = mlx_new_image(data.mlx_ptr, 500 , 500);
	//
	// mlx_put_image_to_window (data.mlx_ptr, data.win_ptr, 0, 0);
	if (key == 2)
		 	 point.x += 20;
		if (key == 0)
			point.x -= 20;
		if (key == 13)
				 point.y -= 20;
		if (key == 1)
				point.y += 20;
		if (key == 38)
			ft_turnX(&point, M_PI/6);
			if (key == 37)
				ft_turnY(&point, M_PI/6);
				if (key == 40)
					ft_turnZ(&point, M_PI/6);
			printf("x:%f y:%f z:%f key%d\n", point.x, point.y, point.z, key);
			ft_draw();
	return (0);
}
int main(void)
{
	    data.w_height = 500;
			data.w_width = 500;
			data.bits_per_pixel  = 32;
			data.size_line = 7680;
			data.endian = 1;


			point.x = 0;
			point.y = 0;
			point.z = 0;

		//	printf("%f\n", sin(M_PI/2));
		    if ((data.mlx_ptr = mlx_init()) == NULL)
				        return (EXIT_FAILURE);
			    if ((data.win_ptr = mlx_new_window(data.mlx_ptr, data.w_height, data.w_width, "Hello world")) == NULL)
					        return (EXIT_FAILURE);

						printf("%d", mlx_key_hook(data.win_ptr, ft_main, (void *)0));
					//	mlx_string_put(data.mlx_ptr, data.win_ptr, j, k, 0xFFFFFF, "HELLO");
				    mlx_loop(data.mlx_ptr);
					    return (EXIT_SUCCESS);
}
