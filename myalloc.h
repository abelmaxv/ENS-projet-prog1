#include <stdlib.h>

void *mymalloc(size_t size);
void myfree(void* ptr);
void *myrealloc(void *ptr, size_t size);

void *mymalloc_v1(size_t size);
void myfree_v1(void* ptr);

int visualise_mem();
int visualise_bloc(int b);
