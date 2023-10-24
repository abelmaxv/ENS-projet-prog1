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
    char tab[] = {1, 2, 3};
    ctrl_write(p, 0, 3, tab);
    printf("\n\nStores the values 1,2,3 in ardress %p \n\n", (void *)p);
    print_bloc(p);
    initialize();
}

void mymalloc_test2()
{
    /*Alocates a bloc of memory of size 1000 then allocates another large bloc.
    Tests subcases 2 and 1 of mymalloc*/
    print_freeLarge();
    size_t *p = (size_t *)mymalloc(1000);
    printf("\n\nAllocates one bloc of memory at adress %p for size 1000 (large and ~ big_free ) \n\n", (void *)p);
    print_freeLarge();
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
    print_freeLarge();
    size_t *p = (size_t *)mymalloc(200);
    printf("\n\nAllocates one bloc of memory at adress %p for size 200 (large and << big_free ) \n\n", (void *)p);
    print_freeLarge();
    print_bloc(p);
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
    for (int i = 0; i < 5000; i++)
    {
        mymalloc(500);
    }
    initialize();
}

void myfree_test1()
{
    /*Allocates a small bloc of memory then frees it*/
    char *p = (char *)mymalloc(1);
    printf("\n\nAllocates a small bloc of memory at adress %p \n\n", (void *)p);
    print_smallOccupiedList();
    myfree(p);
    printf("\n\nFrees bloc of adress %p \n\n", (void *)p);
    print_smallOccupiedList();
    initialize();
}

void myfree_test2()
{
    /*Allocates a large bloc of memory then frees it*/
    char *p = (char *)mymalloc(300);
    printf("\n\nAllocates a large bloc of memory at adress %p \n\n", (void *)p);
    print_freeLarge();
    myfree(p);
    printf("\n\nFrees bloc of adress %p \n\n", (void *)p);
    print_freeLarge();
    initialize();
}

void myfree_test3()
{
    /* Allocates a bloc then tries to free it two times */
    char *p = mymalloc(6);
    printf("\n\n Allocates a small bloc at address %p\n\n", (void *)p);
    print_smallOccupiedList();
    printf("\n\nFrees the ardess %p\n\n", (void *)p);
    myfree(p);
    print_smallOccupiedList();
    printf("\n\nFrees the ardess %p\n\n", (void *)p);
    myfree(p);
    initialize();
}

void myfree_test4()
{
    /* Tries to free a bloc that is not in the table */
    char *p = (char *)malloc(sizeof(char));
    print_freeList();
    printf("\n\nFrees memory at adress %p (random adress)\n\n", (void *)p);
    myfree(p);
    printf("\n\nNOtice the error can't be adapted to the situation\n\n");
    initialize();
}

void myfree_test5()
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
    printf("\n\nAllocates a small bloc at adress %p\n\n", (void *)p);
    print_smallOccupiedList();
    p = myrealloc(p, 6);
    printf("\n\n Reallocates bloc at adress %p with size 6 \n\n", (void *)p);
    print_smallOccupiedList();
    initialize();
}

void myrealloc_test2()
{
    /*Allocates a large bloc then reallocates it for a smaller size*/
    char *p = (char *)mymalloc(300);
    char t[] = {1, 2, 3};
    ctrl_write(p, 0, 3, t);
    printf("\n\nAllocates a bloc of size 300 at adress %p and writes 123 at the begining \n\n", (void *)p);
    print_bloc(p);
    char *q = (char *)myrealloc(p, 200);
    printf("\n\n Reallocates from %p to %p for size 200 \n\n", (void *)p, (void *)q);
    print_bloc(p);
    print_bloc(q);
    initialize();
}

void myrealloc_test3()
{
    /*Allocates a large bloc then reallocates it for a larger size*/
    char *p = (char *)mymalloc(300);
    char t[] = {1, 2, 3};
    ctrl_write(p, 0, 3, t);
    printf("\n\nAllocates a bloc of size 300 at adress %p and writes 123 at the begining \n\n", (void *)p);
    print_bloc(p);
    char *q = (char *)myrealloc(p, 400);
    printf("\n\n Reallocates from %p to %p for size 400 \n\n", (void *)p, (void *)q);
    print_bloc(p);
    print_bloc(q);
    initialize();
}

void perf1_aux(int n, void *(*mymalloc)(size_t), void (*myfree)(void *))
{
    /*Allocates 100 small blocs and frees 100 blocs. Repeted n times*/
    char *ptrs[100];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            ptrs[j] = (char *)(*mymalloc)(1);
        }
        for (int j = 0; j < 100; j++)
        {
            (*myfree)(ptrs[j]);
        }
    }
}

void performance_test1()
{
    /* Compares the time of execution of mymalloc with previous version and stdlib malloc.
    Operates 1 000 000 times MAX_SMALL allocation and free and times it.
    */
    printf("TEST ON SMALL BLOCS \n");
    clock_t start, end;
    double execution_time;
    printf("Testing performance of mymalloc_v1... \n");
    start = clock();
    perf1_aux(10000, &mymalloc_v1, &myfree_v1);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time for mymalloc_v1 : %f sec \n\n", execution_time);
    initialize();

    printf("Testing performance of mymalloc... \n");
    start = clock();
    perf1_aux(10000, &mymalloc, &myfree);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time for mymalloc : %f sec \n\n", execution_time);
    initialize();

    printf("Testing performance of stdlib malloc... \n");
    start = clock();
    perf1_aux(10000, &malloc, &free);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time for malloc : %f sec \n\n", execution_time);
}

void perf2_aux(int n, int *tab, void *(*mymalloc)(size_t), void (*myfree)(void *))
{
    /*Allocates 100 blocs which sizes are indicates by the elements of tab, and free every blocs. Repeated n times */
    char *ptrs[100];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            ptrs[j] = (char *)(*mymalloc)(tab[j]);
        }
        for (int j = 0; j < 100; j++)
        {
            (*myfree)(ptrs[j]);
        }
    }
}

void performance_test2()
{
    /* Compares the time of execution of mymalloc with stdlib malloc.
    Operates 1 000 000 times random size allocation and free and times it.
    */
    printf("TEST ON LARGE BLOCS \n");
    clock_t start, end;
    double execution_time;
    int random_sizes[100];
    srand(time(NULL));
    for (int i = 0; i < 100; i++)
    {
        random_sizes[i] = rand() % 1024 + 128;
    }
    printf("Testing performance of mymalloc... \n");
    start = clock();
    perf2_aux(100, random_sizes, &mymalloc, &myfree);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time for mymalloc : %f sec \n\n", execution_time);
    initialize();

    printf("Testing performance of stdlib malloc... \n");
    start = clock();
    perf2_aux(100, random_sizes, &malloc, &free);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time for stdlib malloc : %f sec \n\n", execution_time);
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

    printf("\n\n\n################################################################### \n");
    printf("########################## TEST MYFREE 1 ##########################\n");
    printf("################################################################### \n\n\n");
    myfree_test1();
    printf("\n\n\n################################################################### \n");
    printf("########################## TEST MYFREE 2 ##########################\n");
    printf("################################################################### \n\n\n");
    myfree_test2();
    printf("\n\n\n################################################################### \n");
    printf("########################## TEST MYFREE 3 ##########################\n");
    printf("################################################################### \n\n\n");
    myfree_test3();
    printf("\n\n\n################################################################### \n");
    printf("########################## TEST MYFREE 4 ##########################\n");
    printf("################################################################### \n\n\n");
    myfree_test4();

    printf("\n\n\n###################################################################### \n");
    printf("########################## TEST MYREALLOC 1 ##########################\n");
    printf("###################################################################### \n\n\n");
    myrealloc_test1();
    printf("\n\n\n###################################################################### \n");
    printf("########################## TEST MYREALLOC 2 ##########################\n");
    printf("###################################################################### \n\n\n");
    myrealloc_test2();
    printf("\n\n\n###################################################################### \n");
    printf("########################## TEST MYREALLOC 3 ##########################\n");
    printf("###################################################################### \n\n\n");
    myrealloc_test3();

    printf("\n\n\n######################################################################## \n");
    printf("########################## TEST PERFORMANCE 1 ##########################\n");
    printf("######################################################################## \n\n\n");
    performance_test1();
    printf("\n\n\n######################################################################## \n");
    printf("########################## TEST PERFORMANCE 2 ##########################\n");
    printf("######################################################################## \n\n\n");
    performance_test2();

    return 0;
}
