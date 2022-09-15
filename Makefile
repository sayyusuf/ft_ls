
NAME = ft_ls

_SRC = main.c dir.c print.c perm.c sort.c destroy.c set.c

_OBJ = $(_SRC:.c=.o)
_EXLIB = libft.a
_EXLIB2 = argv.a
_IDIR = ./ ./libft ./argv_lib

SDIR = 
ODIR = 
EXLIBD = libft/
EXLIBD2 = argv_lib/
IFLAG = -I

SRC = $(patsubst %, $(SDIR)%, $(_SRC))
OBJ = $(patsubst %, $(ODIR)%, $(_OBJ))
EXLIB = $(patsubst %, $(EXLIBD)%, $(_EXLIB))
EXLIB2 = $(patsubst %, $(EXLIBD2)%, $(_EXLIB2))
INC = $(patsubst %, $(IFLAG)%, $(_IDIR))

CC = gcc

FLAGS = -Wall -Wextra -Werror -g -fsanitize=address

.PHONY: all clean fclean re

all: $(EXLIB) $(EXLIB2) $(NAME)



$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(INC) $(OBJ) $(EXLIB) $(EXLIB2) -o $(NAME)

$(ODIR)%.o : $(SDIR)%.c 
	$(CC) $(FLAGS) $(INC) -c  $< -o $@


$(EXLIB):
	make -C $(EXLIBD)

$(EXLIB2):
	make -C $(EXLIBD2)

clean:
	rm -rf $(OBJ)
	make clean -C $(EXLIBD)
	make clean -C $(EXLIBD2)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(EXLIBD)
	make fclean -C $(EXLIBD2)

re: fclean all