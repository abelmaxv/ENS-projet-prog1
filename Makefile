CC = gcc
COPT = -Wall -Wextra -Werror -Wshadow -ansi -pedantic -std=c11 -Wno-deprecated
#-Wno-deprecated was added to allow the use of sbrk() on macOS

myalloc_test : myalloc_test.c myalloc.c
	$(CC) $(COPT) myalloc.c myalloc_test.c -o myalloc_test
