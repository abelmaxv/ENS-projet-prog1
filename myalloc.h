#include <stdlib.h>

void *mymalloc(size_t size);
void myfree(void* ptr);
void *myrealloc(void *ptr, size_t size);

void *mymalloc_v1(size_t size);
void myfree_v1(void* ptr);

void initialize();

void visualize_mem();
void visualize_bloc(int b);
void visualize_free();
