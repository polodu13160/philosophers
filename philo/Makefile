.phony =all re clean fclean

FLAGS = -Wall -Wextra -g3 -MMD -MP -Iincludes/
FLAGS_DEBUG = -DDEBUG=1 -fsanitize=thread
NAME = philo
FILES = main \
		debug \
		times \
		print \
		parsing \
		init \
		free_and_destroy_mutex \
		actions/take_forks \
		actions/change_actions \


OBJ_DIR = objects/
SRC_DIR = sources/
SRCS = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(FILES)))
DEPS = $(addprefix $(OBJ_DIR), $(addsuffix .d, $(FILES)))

all: $(NAME)

debug: FLAGS += $(FLAGS_DEBUG)
debug: fclean all

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/actions

$(OBJ_DIR)%.o: $(SRC_DIR)%.c 
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) 
-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) clean

re: fclean all




