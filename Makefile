CC = gcc
COPT = -Wall -Wextra -Werror -Wshadow -ansi -pedantic -std=c11

myalloc : myalloc.c 
	$(CC) $(COPT) myalloc.c -o myalloc