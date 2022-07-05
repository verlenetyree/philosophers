NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror # -g -fsanitize=thread # -g3 -fsanitize=address
#SANFLAGS =  -g -fsanitize=thread # -g3 -fsanitize=address						$(SANFLAGS)

SOURCES_DIR = ./sources/
SOURCES_LIST = main.c philo.c utils.c time.c
SOURCES = $(addprefix $(SOURCES_DIR), $(SOURCES_LIST))

OBJECTS_DIR = ./objects/
OBJECTS_LIST = $(patsubst %.c, %.o, $(SOURCES_LIST))
OBJECTS = $(addprefix $(OBJECTS_DIR), $(OBJECTS_LIST))

D_FILES_DIR = ./objects/
D_FILES_LIST = $(patsubst %.c, %.d, $(SOURCES_LIST))
D_FILES = $(addprefix $(D_FILES_DIR), $(D_FILES_LIST))

all: $(NAME)

$(NAME): $(OBJECTS_DIR) $(OBJECTS)
	$(CC) $(SANFLAGS) $(OBJECTS) -o $(NAME)

$(OBJECTS_DIR):
	@mkdir -p $(OBJECTS_DIR)

$(OBJECTS_DIR)%.o: $(SOURCES_DIR)%.c
	$(CC) $(CFLAGS)$(SANFLAGS) -c $< -o $@ -MMD

include $(wildcard $(D_FILES_DIR)*.d)

.PHONY: all clean fclean re

clean:
	@rm -rf $(OBJECTS_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
