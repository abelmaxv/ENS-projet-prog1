#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "myalloc.h"

void initialize_test()
{
    /* Initializes memory when calling mymalloc */
    int *p = (int *)mymalloc(4);
    printf("\n\nAllocates a pointer at adress %p which calls initialize()\n\n", (void *)p);
    print_freeList();
    initialize();
}

void mymalloc_test1()
{
    /*Allocates first bloc of memory then puts a value in it*/

    char *p = (char *)mymalloc(1);
    printf("\n\nAllocates one bloc of memory at adress %p for size 1 (small) \n\n", (void *)p);
    print_smallOccupiedList();
    char tab []= {1, 2, 3};
    ctrl_write(p,0,3,tab);
    printf("\n\nStores the values 1,2,3 in ardress %p \n\n", (void *)p);
    print_bloc(p);
    initialize();
}

void mymalloc_test2()
{
    /*Alocates a bloc of memory of size 1000 then allocates another large bloc.
    Tests subcases 2 and 1 of mymalloc*/
    size_t *p = (size_t *)mymalloc(1000);
    printf("\n\nAllocates one bloc of memory at adress %p for size 1000 (large and ~ big_free ) \n\n", (void *)p);
    print_freeList();
    print_bloc(p);
    size_t *q = (size_t *)mymalloc(300);
    printf("\n\nAllocates one bloc of memory at adress %p for size 300 (with no large free bloc avalable) \n\n", (void *)q);
    print_bloc(q);
    initialize();
}

void mymalloc_test3()
{
    /*Allocates a bloc of size  200 
    Tests subcase 3 of mymalloc*/
    size_t *p = (size_t *) mymalloc(200);
    printf("\n\nAllocates one bloc of memory at adress %p for size 200 (large and << big_free ) \n\n", (void *)p);
    print_freeList();
    print_bloc(p);
    initialize();
}

void mymalloc_test7()
{
    /* Allocates 3 blocs of memory, frees the 2de one, then allocates a bloc */
    char *p = (char *)mymalloc(1);
    char *q = (char *)mymalloc(1);
    char *r = (char *)mymalloc(1);
    printf("\n\nAllocates 3 blocs of memory at adresses %p,%p,%p\n\n", (void *)p, (void *)q, (void *)r);
    print_mem();
    myfree(q);
    printf("\n\nFrees bloc of adress %p\n\n", (void *)q);
    print_mem();
    char *s = (char *)mymalloc(1);
    printf("\n\nAllocates 1 bloc of memory at adress %p\n\n", (void *)s);
    print_mem();
    initialize();
}

void mymalloc_test4()
{
    /* Tries to allocates memory 103 times */
    printf("\n\nAllocates small bloc memory 103 times \n\n");
    for (int i = 0; i < 103; i++)
    {
        mymalloc(1);
    }
    print_freeList();
    initialize();
}

void mymalloc_test5()
{
    /* Tries to allocates a bloc of size 1000000000*/
    printf("\n\nAllocates a bloc of size 1000000000 \n\n");
    mymalloc(1000000000);
    initialize();
}

void mymalloc_test6()
{
    /* Tries to allocates large blocs 5000 times*/
    printf("\n\nAllocates blocs of size 500, 5000 times \n\n");
    for (int i = 0 ;i <5000; i++ )
    {
        mymalloc(500);
    }
    initialize();
}

void myfree_test1()
{
    /*Allocates first bloc of memory then frees it*/
    char *p = (char *)mymalloc(1);
    printf("\n\nAllocates 1 bloc of memory at adress %p \n\n", (void *)p);
    print_mem();
    myfree(p);
    printf("\n\nFrees bloc of adress %p \n\n", (void *)p);
    print_mem();
    initialize();
}

void myfree_test2()
{
    /* Allocates a bloc then tries to free it two times */
    char *p = mymalloc(6);
    p[4] = 6;
    printf("\n\n Allocates a bloc at address %p then change it 5th byte to 6\n\n", (void *)p);
    print_mem();
    printf("\n\nFrees the ardess %p twice\n\n", (void *)p);
    myfree(p);
    myfree(p);
    initialize();
}

void myfree_test3()
{
    /* Tries to free a bloc that is not in the table */
    char *p = (char *)malloc(sizeof(char));
    print_freeList();
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
    print_mem();
    p = myrealloc(p, 6);
    printf("\n\n Reallocates bloc at adress %p with size 6 \n\n", (void *)p);
    print_mem();
    printf("\n\n Frees the bloc at adresse %p\n\n", (void *)p);
    myfree(p);
    print_mem();
    initialize();
}

void myrealloc_test2()
{
    /* Tries to realloc a bloc that is not in small_tab */
    char *p = (char *)malloc(sizeof(char));
    print_freeList();
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
    printf("\n\n\n##################################################################### \n");
    printf("########################## TEST INITIALIZE ##########################\n");
    printf("##################################################################### \n\n\n");
    initialize_test();

    printf("\n\n\n##################################################################### \n");
    printf("########################## TEST MYMALLOC 1 ##########################\n");
    printf("##################################################################### \n\n\n");
    mymalloc_test1();
    printf("\n\n\n##################################################################### \n");
    printf("########################## TEST MYMALLOC 2 ##########################\n");
    printf("##################################################################### \n\n\n");
    mymalloc_test2();
    printf("\n\n\n##################################################################### \n");
    printf("########################## TEST MYMALLOC 3 ##########################\n");
    printf("##################################################################### \n\n\n");
    mymalloc_test3();
    printf("\n\n\n##################################################################### \n");
    printf("########################## TEST MYMALLOC 4 ##########################\n");
    printf("##################################################################### \n\n\n");
    mymalloc_test4();
    printf("\n\n\n##################################################################### \n");
    printf("########################## TEST MYMALLOC 5 ##########################\n");
    printf("##################################################################### \n\n\n");
    mymalloc_test5();
    printf("\n\n\n##################################################################### \n");
    printf("########################## TEST MYMALLOC 6 ##########################\n");
    printf("##################################################################### \n\n\n");
    mymalloc_test6();

    // printf("\n\n\n########################## TEST MYFREE 1 ########################## \n\n\n");
    // myfree_test1();
    // printf("\n\n\n########################## TEST MYFREE 2 ########################## \n\n\n");
    // myfree_test2();
    // printf("\n\n\n########################## TEST MYFREE 3 ########################## \n\n\n");
    // myfree_test3();
    // printf("\n\n\n########################## TEST MYFREE 4 ########################## \n\n\n");
    // myfree_test4();

    // printf("\n\n\n########################## TEST MYREALLOC 1 ########################## \n\n\n");
    // myrealloc_test1();
    // printf("\n\n\n########################## TEST MYREALLOC 2 ########################## \n\n\n");
    // myrealloc_test2();

    // printf("\n\n\n########################## TEST PERFORMANCE ########################## \n\n\n");
    // performance_test();

    return 0;
}
