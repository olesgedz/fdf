#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mlx.h>

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

typedef struct char_s
{
	int x;
	int y;
	int size;
} t_char;
int k = 0;
int j = 0;
data_t        data;

static void	img_pixel_put(int x, int y, int color)
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

void ft_draw(int k, int j)
{
	int x = 0;
	int y = 0;
	data.img_ptr = mlx_new_image (data.mlx_ptr, data.w_width, data.w_height);
	data.img_data = mlx_get_data_addr (data.img_ptr, &data.bits_per_pixel, &data.size_line, &data.endian);
	
	while (  y < 100)
	{
		 x = 0;
		while (x < 100)
		{
			//mlx_pixel_put(data.mlx_ptr, data.win_ptr, k+y, j+x, 0xFFFFFF);
			img_pixel_put( k + x, j + y,  0xFFFFFFF);
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
		 	 k += 20;
		if (key == 0)
			k -= 20;
		if (key == 13)
				 j -= 20;
		if (key == 1)
				j += 20;
			printf("x:%d y:%d\n", k, j);
			ft_draw(k,  j);
	
	
	return (0);
} 
int main(void)
{
	    data.w_height = 500;
			data.w_width = 500;
			data.bits_per_pixel  = 32;
			data.size_line = 7680;
			data.endian = 1;

		    if ((data.mlx_ptr = mlx_init()) == NULL)
				        return (EXIT_FAILURE);
			    if ((data.win_ptr = mlx_new_window(data.mlx_ptr, data.w_height, data.w_width, "Hello world")) == NULL)
					        return (EXIT_FAILURE);
					
						printf("%d", mlx_key_hook(data.win_ptr, ft_main, (void *)0));
					//	mlx_string_put(data.mlx_ptr, data.win_ptr, j, k, 0xFFFFFF, "HELLO");
				    mlx_loop(data.mlx_ptr);
					    return (EXIT_SUCCESS);
}
