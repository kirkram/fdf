NAME = fdf
LIBFT_PATH = ./lib/libft
GNL_PATH = ./lib/get_next_line
LIBMLX_PATH	= ./lib/MLX42
LIBFT = ./lib/libft/libft.a
LIBMLX_REPO = https://github.com/codam-coding-college/MLX42.git
LIBMLX42 = $(LIBMLX_PATH)/build/libmlx42.a
CFLAGS = -Wall -Wextra -Werror -Wunreachable-code -Ofast -lm
HEADERS	= -I ./include -I ./libft -I /usr/local/Cellar/glfw/include -I "/Users/$(USER)/.brew/opt/glfw/include" 
DEBUGFLAGS = -g -fsanitize=address,undefined,integer
LIBS	= $(LIBMLX42) -lm -lglfw -L /Users/$(USER)/.brew/opt/glfw -L/Users/$(USER)/.brew/lib -L/opt/homebrew/lib #-framework Cocoa -framework OpenGL -framework IOKit
SRCS = ./src/fdf_main.c ./src/helper_functions.c ./src/ft_atoi_b.c ./src/fdf_reader.c ./src/fdf_get_colors.c \
./src/fdf_colors_to_list.c ./src/fdf_gradient.c ./src/fdf_draw.c ./src/fdf_hook_control_hub.c ./src/fdf_point.c
BONUS_SRCS = ./src/bonus/fdf_main_bonus.c ./src/bonus/fdf_hook_controls_bonus.c ./src/bonus/fdf_hook_control_hub_bonus.c \
./src/helper_functions.c ./src/ft_atoi_b.c ./src/fdf_reader.c ./src/fdf_get_colors.c \
./src/fdf_colors_to_list.c ./src/fdf_gradient.c ./src/fdf_draw.c ./src/fdf_point.c 
GNL_SRCS = ./lib/get_next_line/get_next_line_utils.c ./lib/get_next_line/get_next_line.c
#PRINTF_SRCS = ./lib/ft_printf/ft_printf.c ./lib/ft_printf/ft_putchar.c ./lib/ft_printf/ft_putstr.c
OBJCTS = $(SRCS:.c=.o)
BONUS_OBJCTS = $(BONUS_SRCS:.c=.o)
GNL_OBJCTS = $(GNL_SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(LIBMLX42) $(SRCS) $(GNL_SRCS)
	cc $(CFLAGS) $(HEADERS) $(BONUS_SRCS) $(LIBFT) $(GNL_SRCS) $(LIBS)   -o $(NAME)

$(LIBMLX42): .libmlx42

.libmlx42: clone_mlx42
	@if [ ! -f .libmlx42 ]; then \
		echo "Building MLX42 library..."; \
		cd $(LIBMLX_PATH) && cmake -B build && make -C build -j4 || exit 1; \
		cd ../../; \
		touch .libmlx42; \
	else \
		echo "MLX42 library is already built."; \
	fi

clone_mlx42:
	@if [ ! -d "$(LIBMLX_PATH)" ]; then \
		echo "Cloning MLX42 library..."; \
		git clone $(LIBMLX_REPO) $(LIBMLX_PATH); \
	else \
		echo "MLX42 library already exists."; \
	fi

$(LIBFT):
	make -C $(LIBFT_PATH)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@ $(HEADERS)

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	$(RM) .libmlx42
	$(RM) -r $(LIBMLX_PATH)/build


re: fclean all

.PHONY: all clean fclean re bonus both debug libmlx