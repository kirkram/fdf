NAME = fdf
LIBFT_PATH = ./lib/libft
GNL_PATH = ./lib/get_next_line
LIBMLX_PATH	= ./lib/MLX42
LIBFT = ./lib/libft/libft.a
CFLAGS = -Wall -Wextra -Werror -Wunreachable-code -Ofast
HEADERS	= -I ./include -I ./lib/MLX42/include/ -I ./lib/get_next_line/ -I "/Users/klukiano/.brew/opt/glfw/include"
DEBUGFLAGS = -g -fsanitize=address,undefined,integer
LIBS	= ./lib/MLX42/build/libmlx42.a -L/Users/klukiano/.brew/lib/ -lglfw -framework Cocoa -framework OpenGL -framework IOKit
SRCS = ./src/fdf_main.c ./src/helper_functions.c ./src/ft_atoi_b.c ./src/fdf_reader.c ./src/fdf_get_colors.c ./src/fdf_hook_controls.c
GNL_SRCS = ./lib/get_next_line/get_next_line_utils.c ./lib/get_next_line/get_next_line.c
PRINTF_SRCS = ./lib/ft_printf/ft_printf.c ./lib/ft_printf/ft_putchar.c ./lib/ft_printf/ft_putstr.c
OBJCTS = $(SRCS:.c=.o)
GNL_OBJCTS = $(GNL_SRCS:.c=.o)
RM = rm -f

#all: libmlx $(NAME)

all: $(NAME)

$(NAME): $(LIBFT) $(SRCS) libmlx
	cc $(CFLAGS) $(HEADERS) $(SRCS) $(PRINTF_SRCS) $(LIBFT) $(GNL_SRCS) $(LIBS)   -o $(NAME)

libmlx:
	@cmake -DDEBUG=1 $(LIBMLX_PATH) -B $(LIBMLX_PATH)/build && make -C $(LIBMLX_PATH)/build -j4

$(LIBFT):
	make -C $(LIBFT_PATH)

# bonus: .bonus

# .bonus: $(LIBFT)
# 	cc $(CFLAGS) $(LIBFT) -o
# 	touch .bonus


##RELINKS WHEN MAKE AND THE DEBUG!!

debug: .debug

.debug: libmlx $(SRCS) $(GNL_SRCS) $(PRINTF_SRCS)
	cc $(DEBUGFLAGS) $(HEADERS) $(SRCS) $(GNL_SRCS) $(PRINTF_SRCS) $(LIBFT) $(LIBS)  -o debug.out
	touch .debug

%.o: %.c
	cc $(CFLAGS) -c $< -o $@ $(HEADERS)

both: $(NAME) bonus

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	$(RM) -r $(LIBMLX_PATH)/build
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	$(RM) debug.out
	$(RM) .debug
	$(RM) .bonus

re: fclean all

.PHONY: all clean fclean re bonus