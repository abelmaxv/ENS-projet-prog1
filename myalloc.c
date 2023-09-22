#include <stdlib.h>

#define MAX_SMALL 100

const int SIZE_BLK_SMALL = 128-sizeof(size_t);
char small_tab[128*MAX_SMALL];


void * mymalloc(size_t size){
    if (size<=SIZE_BLK_SMALL){
        for (int b=0; b<MAX_SMALL;b++){
            if(*(size_t*)(small_tab+(128*b))%2==0){
                return small_tab +(128*b)+sizeof(size_t);
            }
        }
    }
    return NULL;
}

void myfree(void* ptr){
    if (*((size_t *) ptr -1)%2 == 0){
        printf("Erreur : le bloc indiqué est libre \n");
    }
    else{
        
    }
}