#include <stdlib.h>
#include <stdio.h>

#define MAX_SMALL 100
#define SIZE_BLK_SMALL (128-sizeof(size_t))

char small_tab[MAX_SMALL*128];
//Begining of the list of free blocs : fst_free
char * fst_free = small_tab;

//is_initialized becomes 1 after the first call of mymalloc and initialize
char is_initialized = 0; 

void initialize(){
    /* Fills headers to make initial list for empty small_tab */
    size_t **ptr;
    for (int b = 0; b<MAX_SMALL-1; b++){
        ptr = (size_t **)(small_tab + b*128);
        *ptr = (size_t *) (small_tab + (b+1)*128);
    }
    ptr =(size_t **) (small_tab + (MAX_SMALL-1)*128);
    *ptr = NULL;
    is_initialized = 1;
}




/* MEMORY ALLOCATION FUNCTIONS */


// First part version
void *mymalloc_v1(size_t size){
    if (size<=SIZE_BLK_SMALL){
        for (size_t b=0; b<MAX_SMALL;b++){
            if(*(size_t*)(small_tab+(128*b))%2==0){
                *(size_t*)(small_tab+(128*b))+=1;
                return small_tab+(128*b)+sizeof(size_t);
            }
        }
        printf("ERROR : none of the blocs are free \n");
    }
    else{
        printf("ERROR : asked for too much memory \n");
    }
    return NULL;
}


void *mymalloc(size_t size){
    //initializes small_tab iif it's not allready done
    if (is_initialized ==0)
        initialize();

    if (size<=SIZE_BLK_SMALL && fst_free != NULL){
            char * temp_ptr = fst_free ;

            // changes fst_free
            fst_free = *(char **) temp_ptr;
            // marks the bloc as occupied
            *(size_t *) temp_ptr +=1;

            return temp_ptr + sizeof(size_t);
    }
    else if (fst_free == NULL)
        printf("ERROR : none of the blocs are free \n");
    else
        printf("ERROR : asked for too much memory \n");
    return NULL;
}







//First part version
void myfree_v1(void* ptr){
    if (!((size_t) small_tab <= (size_t) ptr  && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL)))
        printf("ERROR : the pointer given isn't accessible by myfree \n");
    
    else if(((size_t) ptr - (size_t) small_tab)%128 != sizeof(size_t)){
        printf("ERROR : the pointer given is not at the begining of a bloc \n");
    }
    else if (*((size_t *) ptr -1)%2 == 0){
        printf("ERROR : the bloc is empty \n");
    }
    else{
        (*((size_t *) ptr - 1))<<=1;
    }
}


void myfree(void* ptr){
    if (!((size_t) small_tab <= (size_t) ptr  && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL)))
        printf("ERROR : the pointer given isn't accessible by myfree \n");
    
    else if(((size_t) ptr - (size_t) small_tab)%128 != sizeof(size_t)){
        printf("ERROR : the pointer given is not at the begining of a bloc \n");
    }
    else if (*((size_t *) ptr -1)%2 == 0){
        printf("ERROR : the bloc is empty \n");
    }
    else{
        char **bloc_ptr = (char **)((char *) ptr - sizeof(size_t));
        // The freed bloc points to fst_free
        *bloc_ptr = fst_free;  
        // fst_free points to the freed bloc
        fst_free = (char *) ptr - sizeof(size_t);
    }
}


void *myrealloc(void *ptr, size_t size){
    if (!((size_t) small_tab <= (size_t) ptr && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL))){
        printf("ERROR : the pointer given isn't accessible by myrealloc \n");
    }
    else if(((size_t) ptr - (size_t) small_tab)%128 != sizeof(size_t)){
        printf("ERROR : the pointer given is not at the begining of a bloc \n");
    }
    else if(size>SIZE_BLK_SMALL || *((size_t *) ptr -1)%2==0){
        return NULL;
    }
    return ptr;
}



/* FUNCTIONS TO VISUALIZE MEMORY */

void visualise_bloc(int b){
    /*Displays content of bloc b*/
    if (b>= MAX_SMALL || b<0){
        printf("ERROR : bloc out of range \n");
        exit(1);
    }
    printf("Displaying bloc %d \n", b);
    printf("Header : %ld \n", *((size_t*)(small_tab +128*b))); // If header is even then the bloc is free 
    printf("Content : ");
    for(int i = b*128 + sizeof(size_t) ; i<(b+1)*128 ; i++){
        printf("%d", small_tab[i]);
    }
    printf("\n");
    printf("________________________________________________ \n");
}



int *occ_blocs(){
    /*Displays the occupied blocs and returns an array of free blocs*/
    printf("DISPLAYING OCCUPIED BLOCS : \n");
    int counter = 0;
    for (size_t b = 0; b< MAX_SMALL; b++){
        if (*(size_t *)(small_tab + 128*b)%2==1){
            printf("%lu ; ", b);
            counter++;
        }
    }
    printf("\n");
    printf("________________________________________________ \n");
    // The fist element of ob_tab is counter, then idexes of free blocs
    int *ob_tab = (int *) malloc((counter+1)*sizeof(int));
    ob_tab[0]= counter;
    int tab_index = 1;
    for (size_t b = 0; b< MAX_SMALL; b++){
        if (*(size_t *)(small_tab + 128*b)%2==1){
            ob_tab[tab_index]=b;
            tab_index++;
        }
    }
    return ob_tab;
}


int visualise_mem(){
    printf("################################################ \n");
    int* ob_tab = occ_blocs();
    for (int i = 1 ; i< ob_tab[0]+1 ;i++){
        visualise_bloc(ob_tab[i]);
    }
    free(ob_tab);
    printf("################################################ \n");
    return 0;
}






/* CONTROLLED READING/WRITING FUNCTIONS */

void ctrl_read_int(int *ptr){
    /* Allows reading of *ptr iif 
        #1 ptr points in the content part of a bloc
        #2 the type of ptr is small enough that its content is in one bloc
        #3 the bloc is marked as occupied
    */
    
    size_t size = sizeof(int);
    //Checks #1
    if (!((size_t) small_tab <= (size_t) ptr && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL))){
        printf("ERROR : the pointer given isn't accessible by ctrl_read_int \n");
        exit(1);
    }
    //Checks #2
    else if (((size_t) ptr - (size_t) small_tab)/128!=((size_t) ptr + size - (size_t) small_tab)/128){
        printf("ERROR : the data type is to large \n");
        exit(1);
    }
    //Checks #3
    else if ((*(size_t*)(small_tab + ((size_t)ptr -(size_t)small_tab)/128)%2==0)){
        printf("ERROR : the bloc is free \n");
        exit(1);
    }
    printf("%d", *ptr);
}


void ctrl_read_char(char *ptr){
    /* Allows reading of *ptr iif 
        #1 ptr points in the content part of a bloc
        #2 the type of ptr is small enough that its content is in one bloc
        #3 the bloc is marked as occupied
    */
    
    size_t size = sizeof(char);
    //Checks #1
    if (!((size_t) small_tab <= (size_t) ptr && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL))){
        printf("ERROR : the pointer given isn't accessible by ctrl_read_char \n");
        exit(1);
    }
    //Checks #2
    else if (((size_t) ptr - (size_t) small_tab)/128!=((size_t) ptr + size - (size_t) small_tab)/128){
        printf("ERROR : the data type is to large \n");
        exit(1);
    }
    //Checks #3
    else if ((*(size_t*)(small_tab + ((size_t)ptr -(size_t)small_tab)/128)%2==0)){
        printf("ERROR : the bloc is free \n");
        exit(1);
    }
    printf("%d", *ptr);
}

void ctrl_write_int(int * ptr, int n){
    /* Allows reading of *ptr iif 
        #1 ptr points in the content part of a bloc
        #2 the type of ptr is small enough that its content is in one bloc
        #3 the bloc is marked as occupied
    */
    size_t size = sizeof(int);
    //Checks #1
    if (!((size_t) small_tab <= (size_t) ptr && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL))){
        printf("ERROR : the pointer given isn't accessible by ctrl_write_int \n");
        exit(1);
    }
    //Checks #2
    else if (((size_t) ptr - (size_t) small_tab)/128!=((size_t) ptr + size - (size_t) small_tab)/128){
        printf("ERROR : the data type is to large \n");
        exit(1);
    }
    //Checks #3
    else if ((*(size_t*)(small_tab + ((size_t)ptr -(size_t)small_tab)/128)%2==0)){
        printf("ERROR : the bloc is free \n");
        exit(1);
    }
    *ptr=n;
}


void ctrl_write_char(char * ptr, char n){
    /* Allows reading of *ptr iif 
        #1 ptr points in the content part of a bloc
        #2 the type of ptr is small enough that its content is in one bloc
        #3 the bloc is marked as occupied
    */
    size_t size = sizeof(char);
    //Checks #1
    if (!((size_t) small_tab <= (size_t) ptr && (size_t) ptr <= (size_t) (small_tab + 128*MAX_SMALL))){
        printf("ERROR : the pointer given isn't accessible by ctrl_write_char \n");
        exit(1);
    }
    //Checks #2
    else if (((size_t) ptr - (size_t) small_tab)/128!=((size_t) ptr + size - (size_t) small_tab)/128){
        printf("ERROR : the data type is to large \n");
        exit(1);
    }
    //Checks #3
    else if ((*(size_t*)(small_tab + ((size_t)ptr -(size_t)small_tab)/128)%2==0)){
        printf("ERROR : the bloc is free \n");
        exit(1);
    }
    *ptr=n;
}
