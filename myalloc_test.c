#include <stdlib.h>
#include <stdio.h>
#include "myalloc.h"

void mymalloc_test1(){
    /*Allocates first bloc of memory then puts a value in it*/
    char *p = (char *) mymalloc(1);
    visualise_mem();
    *p = 1;
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

}


int main(){
    //mymalloc_test1();
    //myfree_test1();
    mymalloc_test2();

    return 0;
}
