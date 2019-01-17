#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mlx.h>

typedef struct    data_s
{
	    void          *mlx_ptr;
		    void          *win_ptr;
}                 data_t;

int main(void)
{
	    data_t        data;

		    if ((data.mlx_ptr = mlx_init()) == NULL)
				        return (EXIT_FAILURE);
			    if ((data.win_ptr = mlx_new_window(data.mlx_ptr, 500, 500, "Hello world")) == NULL)
					        return (EXIT_FAILURE);
								int j = 0;
								int k = 0;
						// while (j < 250)
						// {
						// 	k = 0;
						// 	while (k < 250)
						// 	{
						// 		mlx_pixel_put(data.mlx_ptr, data.win_ptr, j, k, 0xFFFFFF);
						// 		k++;
						// 	}
						// 	j++;
						// }
						mlx_string_put(data.mlx_ptr, data.win_ptr, j, k, 0xFFFFFF, "HELLO");
				    mlx_loop(data.mlx_ptr);
					    return (EXIT_SUCCESS);
}
