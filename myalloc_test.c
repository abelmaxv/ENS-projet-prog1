#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "myalloc.h"

void initialize_test()
{
    /* Initializes memory when calling mymalloc */
    int *p = (int *)mymalloc(4);
    printf("\n\nAllocates a pointer at adress %p which calls initialize()\n\n", (void *)p);
    visualize_free();
    initialize();
}

void mymalloc_test1()
{
    /*Allocates first bloc of memory then puts a value in it*/

    char *p = (char *)mymalloc(1);
    printf("\n\nAllocates one bloc of memory at adress %p \n\n", (void *)p);
    visualize_mem();
    *p = 1;
    printf("\n\nStores the value 1 in ardress %p \n\n", (void *)p);
    visualize_mem();
    initialize();
}

void mymalloc_test2()
{
    /* Allocates 3 blocs of memory, frees the 2de one, then allocates a bloc */
    char *p = (char *)mymalloc(1);
    char *q = (char *)mymalloc(1);
    char *r = (char *)mymalloc(1);
    printf("\n\nAllocates 3 blocs of memory at adresses %p,%p,%p\n\n", (void *)p, (void *)q, (void *)r);
    visualize_mem();
    myfree(q);
    printf("\n\nFrees bloc of adress %p\n\n", (void *)q);
    visualize_mem();
    char *s = (char *)mymalloc(1);
    printf("\n\nAllocates 1 bloc of memory at adress %p\n\n", (void *)s);
    visualize_mem();
    initialize();
}

void mymalloc_test3()
{
    /* Tries to allocate a data too big */
    visualize_free();
    printf("\n\nTries to allocates object of size 130 \n\n");
    char *p = (char *)mymalloc(130);
    if (p != NULL)
        *p = 1;
    visualize_free();
    initialize();
}

void mymalloc_test4()
{
    /* Tries to allocates memory 103 times */
    printf("\n\nAllocates memory 103 times \n\n");
    for (int i = 0; i < 103; i++)
    {
        mymalloc(1);
    }
    initialize();
}

void myfree_test1()
{
    /*Allocates first bloc of memory then frees it*/
    char *p = (char *)mymalloc(1);
    printf("\n\nAllocates 1 bloc of memory at adress %p \n\n", (void *)p);
    visualize_mem();
    myfree(p);
    printf("\n\nFrees bloc of adress %p \n\n", (void *)p);
    visualize_mem();
    initialize();
}

void myfree_test2()
{
    /* Allocates a bloc then tries to free it two times */
    char *p = mymalloc(6);
    p[4] = 6;
    printf("\n\n Allocates a bloc at address %p then change it 5th byte to 6\n\n", (void *)p);
    visualize_mem();
    printf("\n\nFrees the ardess %p twice\n\n", (void *)p);
    myfree(p);
    myfree(p);
    initialize();
}

void myfree_test3()
{
    /* Tries to free a bloc that is not in the table */
    char *p = (char *)malloc(sizeof(char));
    visualize_free();
    printf("\n\n Frees memory at adress %p\n\n", (void *)p);
    myfree(p);
    initialize();
}

void myfree_test4()
{
    /* Tries to free NULL pointer */
    printf("\n\nFrees NULL pointer\n\n");
    myfree(NULL);
    initialize();
}

void myrealloc_test1()
{
    /* Allocate a bloc then reallocates it then frees it */
    char *p = (char *)mymalloc(1);
    printf("\n\nAllocates a bloc at adress %p\n\n", (void *)p);
    visualize_mem();
    p = myrealloc(p, 6);
    printf("\n\n Reallocates bloc at adress %p with size 6 \n\n", (void *)p);
    visualize_mem();
    printf("\n\n Frees the bloc at adresse %p\n\n", (void *)p);
    myfree(p);
    visualize_mem();
    initialize();
}

void myrealloc_test2()
{
    /* Tries to realloc a bloc that is not in small_tab */
    char *p = (char *)malloc(sizeof(char));
    visualize_free();
    printf("\n\nReallocates memory at adress %p\n\n", (void *)p);
    myrealloc(p, 6);
    initialize();
}

void performance_test()
{
    /* Compares the time of execution of mymalloc with previous version and real malloc.
    Operates 1 000 000 times MAX_SMALL allocation and free and times it.
    */
    clock_t start, end;
    double execution_time;
    char *ptrs[100];
    printf("Testing performance of mymalloc_v1... \n");
    start = clock();
    for (int i = 0; i < 1000000; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            ptrs[j] = (char *)mymalloc_v1(1);
        }
        for (int j = 0; j < 100; j++)
        {
            myfree_v1(ptrs[j]);
        }
    }
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time for mymalloc_v1 : %f sec \n\n", execution_time);
    initialize();

    printf("Testing performance of mymalloc... \n");
    start = clock();
    for (int i = 0; i < 1000000; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            ptrs[j] = (char *)mymalloc(1);
        }
        for (int j = 0; j < 100; j++)
        {
            myfree(ptrs[j]);
        }
    }
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time for mymalloc : %f sec \n\n", execution_time);
    initialize();

    printf("Testing performance of malloc... \n");
    start = clock();
    for (int i = 0; i < 1000000; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            ptrs[j] = (char *)malloc(1);
        }
        for (int j = 0; j < 100; j++)
        {
            free(ptrs[j]);
        }
    }
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time for malloc : %f sec \n\n", execution_time);
}

int main()
{
    printf("\n\n\n########################## TEST INITIALIZE ########################## \n\n\n");
    initialize_test();

    printf("\n\n\n########################## TEST MYMALLOC 1 ########################## \n\n\n");
    mymalloc_test1();
    printf("\n\n\n########################## TEST MYMALLOC 2 ########################## \n\n\n");
    mymalloc_test2();
    printf("\n\n\n########################## TEST MYMALLOC 3 ########################## \n\n\n");
    mymalloc_test3();
    printf("\n\n\n########################## TEST MYMALLOC 4 ########################## \n\n\n");
    mymalloc_test4();

    printf("\n\n\n########################## TEST MYFREE 1 ########################## \n\n\n");
    myfree_test1();
    printf("\n\n\n########################## TEST MYFREE 2 ########################## \n\n\n");
    myfree_test2();
    printf("\n\n\n########################## TEST MYFREE 3 ########################## \n\n\n");
    myfree_test3();
    printf("\n\n\n########################## TEST MYFREE 4 ########################## \n\n\n");
    myfree_test4();

    printf("\n\n\n########################## TEST MYREALLOC 1 ########################## \n\n\n");
    myrealloc_test1();
    printf("\n\n\n########################## TEST MYREALLOC 2 ########################## \n\n\n");
    myrealloc_test2();

    printf("\n\n\n########################## TEST PERFORMANCE ########################## \n\n\n");
    performance_test();

    return 0;
}
