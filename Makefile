NAME = fdf
LIBFT_PATH = ./lib/libft
GNL_PATH = ./lib/get_next_line
LIBMLX_PATH	= ./lib/MLX42
LIBFT = $(LIBFT_PATH)/libft.a
CFLAGS = -Wall -Wextra -Werror
HEADERS	= -I ./include -I $(LIBMLX)/include
#EXTRA FLAGS CFLAGS	:= -Wunreachable-code -Ofast
DEBUGFLAGS = -g -fsanitize=address,undefined,integer #Also Debug MLX
LIBS	= $(LIBMLX)/build/libmlx42.a -L/Users/klukiano/.brew/lib/ -lglfw -framework Cocoa -framework OpenGL -framework IOKit
SRCS = ./src/....
GNL_SRCS = ./lib/get_next_line/get_next_line_util.c ./lib/get_next_line/main_get_next_line.c ./lib/get_next_line/get_next_line.c
OBJCTS = $(SRCS:.c=.o)
GNL_OBJCTS = $(GNL_SRCS:.c=.o)
RM = rm -f

all: libmlx $(NAME)

$(NAME): $(LIBFT)
	cc $(CFLAGS) $(SRCS) $(GNL_SRCS) $(LIBS) $(LIBFT) -o $(NAME)

libmlx:
	@cmake $(LIBMLX_PATH) -B $(LIBMLX_PATH)/build && make -C $(LIBMLX_PATH)/build -j4

$(LIBFT):
	make -C $(LIBFT_PATH)

bonus: .bonus

.bonus: $(LIBFT)
	cc $(CFLAGS) $(LIBFT) -o
	touch .bonus

debug: $(LIBFT)
	cc $(DEBUGFLAGS) $(SRCS) $(PRINTF_SRCS) $(LIBFT) -o debug.out

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

both: $(NAME) bonus

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME) push_swap debug.out checker
	$(RM) .bonus

re: fclean all

.PHONY: all clean fclean re bonus