#include <stdlib.h>

#define MAX_SMALL 100

const int SIZE_BLK_SMALL = 128-sizeof(size_t);
static char small_tab[128*MAX_SMALL];


void * mymalloc(size_t size){
    if (size<=SIZE_BLK_SMALL){
        for (int b=0; b<MAX_SMALL;b++){
            if(*(size_t*)(small_tab+(128*b))%2==0){
                return small_tab+(128*b)+sizeof(size_t);
            }
        }
    }
    return NULL;
}

void myfree(void* ptr){
    if (!((size_t) small_tab <= (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL)))
        printf("Error : the pointer given isn't accessible by myfree \n");
    
    else if(((size_t) ptr - (size_t) small_tab)%128 != SIZE_BLK_SMALL)
        printf("Error : the pointer given is not at the begining of a bloc");
    
    else if (*((size_t *) ptr -1)%2 == 0){
        printf("Error : the bloc is empty \n");
    }
    else{
        *((size_t *) ptr - 1)<<1;
    }
}