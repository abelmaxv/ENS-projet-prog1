#include <stdlib.h>
#include <stdio.h>

#define MAX_SMALL 100

const size_t SIZE_BLK_SMALL = 128-sizeof(size_t);
char small_tab[MAX_SMALL*128]; 


void *mymalloc(size_t size){
    if (size<=SIZE_BLK_SMALL){
        for (size_t b=0; b<MAX_SMALL;b++){
            if(*(size_t*)(small_tab+(128*b))%2==0){
                return small_tab+(128*b)+sizeof(size_t);
            }
        }
    }
    return NULL;
}

void myfree(void* ptr){
    if (!((size_t) small_tab <= (size_t) ptr  && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL)))
        printf("Error : the pointer given isn't accessible by myfree \n");
    
    else if(((size_t) ptr - (size_t) small_tab)%128 != SIZE_BLK_SMALL)
        printf("Error : the pointer given is not at the begining of a bloc");
    
    else if (*((size_t *) ptr -1)%2 == 0){
        printf("Error : the bloc is empty \n");
    }
    else{
        (*((size_t *) ptr - 1))<<=1;
    }
}

void *myrealloc(void *ptr, size_t size){
    if (!((size_t) small_tab <= (size_t) ptr && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL))){
        printf("Error : the pointer given isn't accessible by myfree \n");
    }
    else if(((size_t) ptr - (size_t) small_tab)%128 != SIZE_BLK_SMALL){
        printf("Error : the pointer given is not at the begining of a bloc");
    }
    else if(size>SIZE_BLK_SMALL || *((size_t *) ptr -1)%2==0){
        return NULL;
    }
    return ptr;
}

int visualise_mem(int b){
    /*Displays content of bloc b*/
    if (b>= MAX_SMALL || b<0){
        printf("Error : bloc out of range \n");
        return 1;
    }
    printf("Header : %ld \n", *((size_t*)small_tab +b));
    printf("Content : ");
    for(int i = b*128 ; i<(b+1)*128 ; i++){
        printf("%d", small_tab[i]);
    }
    printf("\n");
    return 0;
}

