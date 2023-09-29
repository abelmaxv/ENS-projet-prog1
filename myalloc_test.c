#include <stdlib.h>
#include <stdio.h>
#include "myalloc.h"

void mymalloc_test1(){
    /*Tests a bloc is marked as occupied after mymalloc */
    char *p = (char *) mymalloc(1);
    visualise_mem(0);
    *p = 1;
    visualise_mem(0);
}


int main(){
    mymalloc_test1();
    return 0;
}