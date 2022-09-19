
NAME = ft_ls

_SRC = main.c dir.c print.c perm.c sort.c destroy.c set.c

_SRC2 = array.c array_insert.c array_del.c array_utils.c array_copy.c  argv.c argv_utils.c argv_insert.c argv_del.c

_OBJ = $(_SRC:.c=.o)
_OBJ2 = $(_SRC2:.c=.o)

_EXLIB = libft.a
_IDIR = ./ ./libft ./argv_lib

SDIR = 
ODIR =

SDIR2 = argv 
ODIR2 =  argv/

EXLIBD = libft/

IFLAG = -I

SRC = $(patsubst %, $(SDIR)%, $(_SRC))
OBJ = $(patsubst %, $(ODIR)%, $(_OBJ))

SRC2 = $(patsubst %, $(SDIR2)%, $(_SRC2))
OBJ2 = $(patsubst %, $(ODIR2)%, $(_OBJ2))

EXLIB = $(patsubst %, $(EXLIBD)%, $(_EXLIB))
INC = $(patsubst %, $(IFLAG)%, $(_IDIR))

CC = gcc

FLAGS = -Wall -Wextra -Werror -g -fsanitize=address

.PHONY: all clean fclean re

all: $(EXLIB) $(NAME)



$(NAME): $(OBJ) $(OBJ2)
	$(CC) $(FLAGS) $(INC) $(OBJ) $(OBJ2) $(EXLIB) $(EXLIB2) -o $(NAME)

$(ODIR)%.o : $(SDIR)%.c 
	$(CC) $(FLAGS) $(INC) -c  $< -o $@


$(EXLIB):
	make -C $(EXLIBD)

clean:
	rm -rf $(OBJ)
	rm -rf $(OBJ2)
	make clean -C $(EXLIBD)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(EXLIBD)

re: fclean all