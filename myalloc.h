#include <stdlib.h>

void initialize();
/******************************* MEMORY ALLOCATION FUNCTIONS *******************************/
void *mymalloc_v1(size_t size);
void *mymalloc_v2(size_t size);
void *mymalloc(size_t size);

void myfree_v1(void *ptr);
void myfree_v2(void *ptr);
void myfree(void *ptr);

void *myrealloc_v1(void *ptr, size_t size);
void *myrealloc(void *ptr, size_t size);

/******************************* FUNCTIONS TO VISUALIZE MEMORY *******************************/

void print_bloc(void *ptr);
void print_freeList();
void print_smallOccupiedList();
void print_mem();

/******************************* CONTROLLED READING/WRITING FUNCTIONS *******************************/

void ctrl_read(void *ptr, size_t offset, size_t size);
void ctrl_write(void *ptr, size_t offset, size_t size, char *src);
