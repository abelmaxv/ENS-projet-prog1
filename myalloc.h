#include <stdlib.h>

void *mymalloc(size_t size);
void myfree(void* ptr);
void *myrealloc(void *ptr, size_t size);

int visualise_mem();
