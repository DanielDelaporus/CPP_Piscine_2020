##
## EPITECH PROJECT, 2019
## CPool_Day10_2019
## File description:
## No file there, just an epitech header example
##
USER = Daniel Delaporus
CC= gcc
EXEC= a.out
FLAGS= -Wall -Wextra -Werror
SRC= $(shell find *.c)
OBJ= $(SRC:.c=.o)
TEST		=	tests/unit_test.c
TEST_FLAG	=	--coverage -lcriterion
TEST_NAME	=	unit_test

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(FLAGS) -o $(EXEC) $(SRC)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(EXEC)
	rm -f *.gcda
	rm -f *.gcno
	rm -f $(TEST_NAME)


re: fclean all

tests_run:	$(OBJ_TEST)
	$(CC) $(FLAGS) -o $(TEST_NAME) $(TEST) $(TEST_FLAG) && ./$(TEST_NAME)

auteur:
		echo $(USER) > auteur

.PHONY: re clean fclean
