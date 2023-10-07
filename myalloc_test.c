#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "myalloc.h"

void mymalloc_test1(){
    /*Allocates first bloc of memory then puts a value in it*/
    char *p = (char *) mymalloc(1);
    visualise_mem();
    *p = 1;
    visualise_mem();

    /* Test passed */
}


void mymalloc_test2(){
    /* Allocates 3 blocs of memory, frees the 2de one, then allocates a bloc */
    char *p = (char *) mymalloc(1);
    *p =1;
    char *q = (char *) mymalloc(1);
    char *r = (char *) mymalloc(1);
    *r =1;
    visualise_mem();
    myfree(q);
    visualise_mem();
    char *s = (char *) mymalloc(1);
    *s =1;
    visualise_mem();

    /* Test passed */

}

void mymalloc_test3(){
    /* Tries to allocate a data too big */
    char *p = (char *)mymalloc(130);
    if(p != NULL)
        *p=1;

    /* Test passed */
}

void mymalloc_test4(){
    /* Tries to allocates memory 101 times */
    for (int i =0; i<101; i++){
        mymalloc(1);
    }
    visualise_mem();
    /* Test passed */
}


void myfree_test1(){
    /*Allocates first bloc of memory then frees it*/
    char *p = (char *) mymalloc(1);
    visualise_mem();
    myfree(p);
    visualise_mem();

    /* Test passed */
}

void myfree_test2(){
    /* Allocates a bloc then tries to free it two times */
    char *p = mymalloc(6);
    p[4]=6;
    visualise_mem();
    myfree(p);
    myfree(p);

    /* Test passed */
}

void myfree_test3(){
    /* Tries to free a bloc that is not in the table */
    char *p = (char *) malloc(sizeof(char));
    myfree(p); 

    /* Test passed */
}

void myfree_test4(){
    /* Tries to free NULL pointer */
    myfree(NULL);

    /* Test passed */
}

void myrealloc_test1(){
    /* Allocate a bloc then reallocates it then frees it */
    char *p = (char *) mymalloc(1);
    visualise_mem();
    p = myrealloc(p,6);
    visualise_mem();
    myfree(p);
    visualise_mem();

    /* Test passed */
}

void myrealloc_test2(){
    /* Tries to realloc a bloc that is not in small_tab */
    char *p = (char *)malloc(sizeof(char));
    myrealloc(p, 6);

    /* Test passed */
}

void initialize_test(){
    /* Initializes memory when calling mymalloc */
    int *p = (int*) mymalloc(4);
    *p +=1;
    for (int i =0 ; i<100 ; i++){
        visualise_bloc(i);
    }
}

void performance_test(){
    /* Compares the time of execution of mymalloc with previous version and real malloc.
    Operates 1 000 000 times MAX_SMALL allocation and free and times it.
    */
    clock_t start, end;
    double execution_time;
    char *ptrs[100];
    printf("Testing performance of mymalloc v1... \n");
    start = clock();
    for(int i=0; i<1000000; i++){
        for (int j=0; j<100; j++){
            ptrs[j]= (char*) mymalloc_v1(1);
        }
        for (int j=0; j<100; j++){
            myfree_v1(ptrs[j]);
        }
    }
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Time for mymalloc_v1 : %f sec \n\n",execution_time);

    printf("Testing performance of mymalloc... \n");
    start = clock();
    for(int i=0; i<1000000; i++){
        for (int j=0; j<100; j++){
            ptrs[j]= (char*) mymalloc(1);
        }
        for (int j=0; j<100; j++){
            myfree(ptrs[j]);
        }
    }
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Time for mymalloc : %f sec \n\n",execution_time);


    printf("Testing performance of malloc... \n");
    start = clock();
    for(int i=0; i<1000000; i++){
        for (int j=0; j<100; j++){
            ptrs[j]= (char*) malloc(1);
        }
        for (int j=0; j<100; j++){
            free(ptrs[j]);
        }
    }
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    printf("Time for malloc : %f sec \n\n",execution_time);


}


int main(){
    /* Uncomment the test to operate */

    //mymalloc_test1();
    //mymalloc_test2();
    //mymalloc_test3();
    //mymalloc_test4();

    //myfree_test1();
    //myfree_test2();
    //myfree_test3();
    //myfree_test4();

    //myrealloc_test1();
    //myrealloc_test2();

    //initialize_test();

    performance_test();
    return 0;
}
