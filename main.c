#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mlx.h>

typedef struct    data_s
{
	void          *mlx_ptr;
	void          *win_ptr;
	void  		*img_ptr;
	char 		*img_data;
	int width;
	int height;
	int bpp;
	int size_line;
	int endian;
}                 data_t;


int k = 0;
int j = 0;
data_t        data;


void ft_putpixel(data_t *data,int x, int y, int color)
{
	((int*)data->img_data)[x + y * data->width] = color;

}

void ft_render(data_t *data)
{
	data->img_ptr =  mlx_new_image(data->mlx_ptr, data->width , data->height);
	data->img_data = mlx_get_data_addr(data->img_ptr, &data->bpp, &data->size_line, &data->endian);

	for (int j = 0; j < 250; j++)
	{
		for (int k = 0; k < 250; k++)
		{
			ft_putpixel(data, k, j, k << 16 | j << 8);
		}
	}



	mlx_put_image_to_window (data->mlx_ptr, data->win_ptr, data->img_ptr, 0, 0);
}

int ft_press(int key, void *data_void)
{

	
	return (0);
} 
int main(void)
{
	    
	data.width = 500;
	data.height = 500;
	if ((data.mlx_ptr = mlx_init()) == NULL)
				return (EXIT_FAILURE);
	if ((data.win_ptr = mlx_new_window(data.mlx_ptr, data.width, data.height, "Hello world")) == NULL)
				return (EXIT_FAILURE);

	ft_render(&data);
	printf("%d", mlx_key_hook(data.win_ptr, ft_press, &data));
	mlx_loop(data.mlx_ptr);
	return (EXIT_SUCCESS);
}
