NAME = fdf
LIBFT_PATH = ./lib/libft
GNL_PATH = ./lib/get_next_line
LIBMLX_PATH	= ./lib/MLX42
LIBFT = ./lib/libft/libft.a
LIBMLX42 = ./lib/MLX42/build/libmlx42.a
CFLAGS = -Wall -Wextra -Werror -Wunreachable-code -Ofast -lm
HEADERS	= -I ./include -I ./lib/MLX42/include/ -I ./lib/get_next_line/ -I "/Users/klukiano/.brew/opt/glfw/include"
DEBUGFLAGS = -g -fsanitize=address,undefined,integer
LIBS	= ./lib/MLX42/build/libmlx42.a -L/Users/klukiano/.brew/lib/ -lglfw -framework Cocoa -framework OpenGL -framework IOKit
SRCS = ./src/fdf_main.c ./src/helper_functions.c ./src/ft_atoi_b.c ./src/fdf_reader.c ./src/fdf_get_colors.c \
./src/fdf_colors_to_list.c ./src/fdf_gradient.c ./src/fdf_draw.c ./src/fdf_hook_control_hub.c ./src/fdf_point.c
BONUS_SRCS = ./src/bonus/fdf_main_bonus.c ./src/bonus/fdf_hook_controls_bonus.c ./src/bonus/fdf_hook_control_hub_bonus.c \
./src/helper_functions.c ./src/ft_atoi_b.c ./src/fdf_reader.c ./src/fdf_get_colors.c \
./src/fdf_colors_to_list.c ./src/fdf_gradient.c ./src/fdf_draw.c ./src/fdf_point.c 
GNL_SRCS = ./lib/get_next_line/get_next_line_utils.c ./lib/get_next_line/get_next_line.c
PRINTF_SRCS = ./lib/ft_printf/ft_printf.c ./lib/ft_printf/ft_putchar.c ./lib/ft_printf/ft_putstr.c
OBJCTS = $(SRCS:.c=.o)
BONUS_OBJCTS = $(BONUS_SRCS:.c=.o)
GNL_OBJCTS = $(GNL_SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(LIBMLX42) $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS)
	cc $(CFLAGS) $(HEADERS) $(SRCS) $(PRINTF_SRCS) $(LIBFT) $(GNL_SRCS) $(LIBS)   -o $(NAME)

$(LIBMLX42): .libmlx42

.libmlx42:
	@cmake $(LIBMLX_PATH) -B $(LIBMLX_PATH)/build && make -C $(LIBMLX_PATH)/build -j4
	touch .libmlx42

$(LIBFT):
	make -C $(LIBFT_PATH)

debug: .debug

.debug: $(LIBMLX42) $(SRCS) $(GNL_SRCS) $(PRINTF_SRCS)
	cc $(DEBUGFLAGS) $(HEADERS) $(SRCS) $(GNL_SRCS) $(PRINTF_SRCS) $(LIBFT) $(LIBS)  -o debug.out
	touch .debug

%.o: %.c
	cc $(CFLAGS) -c $< -o $@ $(HEADERS)

bonus: .bonus

.bonus: $(LIBFT) $(SRCS) $(LIBMLX42) 
	cc $(CFLAGS) $(HEADERS) $(BONUS_SRCS) $(PRINTF_SRCS) $(LIBFT) $(GNL_SRCS) $(LIBS)   -o $(NAME)
	touch .bonus

both: $(NAME) bonus

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	$(RM) debug.out
	$(RM) .debug
	$(RM) .bonus
	$(RM) .libmlx42
	$(RM) -r $(LIBMLX_PATH)/build


re: fclean all

.PHONY: all clean fclean re bonus both debug libmlx