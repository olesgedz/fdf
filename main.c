#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mlx.h>

typedef struct    data_s
{
	    void          *mlx_ptr;
		    void          *win_ptr;
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
int ft_render(int key, void *param)
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
	int x = 0;
	int y = 0;
		//mlx_pixel_put(data.mlx_ptr, data.win_ptr, k, j, 0xFFFFFF);
	
	while (  y < 10)
	{
		 x = 0;
		while (x < 10)
		{
			mlx_pixel_put(data.mlx_ptr, data.win_ptr, k+y, j+x, 0xFFFFFF);
			x++;
		}
		y++;
	}
	
	
	return (0);
} 
int main(void)
{
	    

		    if ((data.mlx_ptr = mlx_init()) == NULL)
				        return (EXIT_FAILURE);
			    if ((data.win_ptr = mlx_new_window(data.mlx_ptr, 500, 500, "Hello world")) == NULL)
					        return (EXIT_FAILURE);
					
						printf("%d", mlx_key_hook(data.win_ptr, ft_render, (void *)0));
					//	mlx_string_put(data.mlx_ptr, data.win_ptr, j, k, 0xFFFFFF, "HELLO");
				    mlx_loop(data.mlx_ptr);
					    return (EXIT_SUCCESS);
}
