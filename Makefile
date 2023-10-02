CC = gcc
COPT = -Wall -Wextra -Werror -Wshadow -ansi -pedantic -std=c11

myalloc_test : myalloc_test.c myalloc.c
	$(CC) $(COPT) myalloc.c myalloc_test.c -o myalloc_test
