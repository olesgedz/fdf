
NAME = fdf

all:
	gcc main.c ./minilibx_macos/libmlx.a -framework OpenGL -framework AppKit -o $(NAME) -I ./minilibx_macos/
