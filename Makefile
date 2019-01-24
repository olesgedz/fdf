
NAME = fdf

all:
	gcc -g  main.c ./libft/libft.a ./minilibx_macos/libmlx.a -framework OpenGL -framework AppKit -o $(NAME) -I ./minilibx_macos/ -I ./libft/includes/
