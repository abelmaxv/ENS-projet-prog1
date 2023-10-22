#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_SMALL 100
#define SIZE_BLK_SMALL (128 - sizeof(size_t))
#define SIZE_BLK_LARGE 1024

// One may need to comment ot uncomment the following to compile on linux or macOS
// void *sbrk(intptr_t increment);

char small_tab[MAX_SMALL * 128];
// Begining of the list of free small blocs : fst_free
char *small_free;
// Begining of list of free large blocs : big_free
size_t *big_free;
// is_initialized becomes 1 after the first call of mymalloc and initialize
char is_initialized = 0;

void initialize()
{
    /* Fills headers to make initial list for empty small_tab */
    small_free = small_tab;
    big_free = (size_t *)sbrk(SIZE_BLK_LARGE);

    if (big_free == (void *)-1)
    {
        printf("ERROR : Could not initialize big_free \n");
        exit(1);
    }

    size_t **ptr;
    // Initializes list of small free blocs :
    for (int b = 0; b < MAX_SMALL - 1; b++)
    {
        ptr = (size_t **)(small_tab + b * 128);
        *ptr = (size_t *)(small_tab + (b + 1) * 128);
    }
    ptr = (size_t **)(small_tab + (MAX_SMALL - 1) * 128);
    *ptr = NULL;

    // Initializes list of large free blocs
    ptr = (size_t **)big_free;
    *ptr = NULL;
    *(big_free + 1) = SIZE_BLK_LARGE;
    is_initialized = 1;
}

/******************************* MEMORY ALLOCATION FUNCTIONS *******************************/

// First part version
void *mymalloc_v1(size_t size)
{
    if (size <= SIZE_BLK_SMALL)
    {
        for (size_t b = 0; b < MAX_SMALL; b++)
        {
            if (*(size_t *)(small_tab + (128 * b)) % 2 == 0)
            {
                *(size_t *)(small_tab + (128 * b)) += 1;
                return small_tab + (128 * b) + sizeof(size_t);
            }
        }
        printf("ERROR : none of the blocs are free \n");
    }
    else
    {
        printf("ERROR : asked for too much memory \n");
    }
    return NULL;
}

// Second part version
void *mymalloc_v2(size_t size)
{
    // initializes small_tab iif it's not allready done
    if (is_initialized == 0)
        initialize();

    if (size <= SIZE_BLK_SMALL && small_free != NULL)
    {
        char *temp_ptr = small_free;

        // changes small_free
        small_free = *(char **)temp_ptr;
        // marks the bloc as occupied
        *(size_t *)temp_ptr += 1;

        return temp_ptr + sizeof(size_t);
    }
    else if (small_free == NULL)
        printf("ERROR : none of the blocs are free \n");
    else
        printf("ERROR : asked for too much memory \n");
    return NULL;
}

// Final version
void *mymalloc(size_t size)
{
    // initializes small_tab iif it's not allready done
    if (is_initialized == 0)
        initialize();

    // Case I : Asking for small size
    if (size <= SIZE_BLK_SMALL && small_free != NULL)
    {
        char *temp_ptr = small_free;

        // changes small_free
        small_free = *(char **)temp_ptr;
        // marks the bloc as occupied
        *(size_t *)temp_ptr += 1;

        return temp_ptr + sizeof(size_t);
    }
    else if (small_free == NULL)
    {
        printf("ERROR : none of the blocs are free \n");
        return NULL;
    }
    // Case II : Asking for large size
    else
    {
        size_t l = size;
        if (l % sizeof(size_t) != 0)
        {
            // l becomes the smallest multiple of sizeof(size_t) greater than size
            l = sizeof(size_t) * (l / sizeof(size_t) + 1);
        }
        // Seeks for a free large bloc of size > l+2*sizeof(size_t)
        size_t *previous_ptr = big_free;
        size_t *ptr = big_free;
        while (ptr != NULL)
        {
            if (*(ptr + 1) > l + 2 * sizeof(size_t))
            {
                break;
            }
            previous_ptr = ptr;
            ptr = *(size_t **)ptr;
        }
        // Subcase 1: If no such bloc was found
        if (ptr == NULL)
        {
            // Ask for a new bloc in memory
            ptr = sbrk(l + 2 * sizeof(size_t));
            if (ptr == (void *)-1)
            {
                printf("ERROR : Could not create a bloc \n");
                return NULL;
            }
            *ptr = (*ptr<<1)+1;
            *(ptr + 1) = l + 2 * sizeof(size_t);
        }

        // Subcase 2: If the bloc found is almost the size asked
        else if (*(ptr + 1) < l + 2 * sizeof(size_t) + SIZE_BLK_SMALL)
        {
            // Declares the bloc as occupied
            if (previous_ptr == big_free)
            {
                big_free = *(size_t **)ptr;
            }
            *(size_t **)previous_ptr = *(size_t **)ptr;
            *ptr += 1;
        }

        // Subcase 3: If the bloc found is far larger than the size asked
        else
        {
            // Divides the bloc
            size_t k = l + 2 * sizeof(size_t);
            size_t new_size = *(ptr + 1) - k;
            *(ptr + 1) = new_size;
            ptr = (size_t *)((char *)ptr + new_size);
            *ptr = 1;
            *(ptr + 1) = k;
        }
        return ptr + 2;
    }
}

// First part version
void myfree_v1(void *ptr)
{
    if (!((size_t)small_tab <= (size_t)ptr && (size_t)ptr <= (size_t)(small_tab + 128 * MAX_SMALL)))
        printf("ERROR : the pointer given isn't accessible by myfree \n");

    else if (((size_t)ptr - (size_t)small_tab) % 128 != sizeof(size_t))
    {
        printf("ERROR : the pointer given is not at the begining of a bloc \n");
    }
    else if (*((size_t *)ptr - 1) % 2 == 0)
    {
        printf("ERROR : the bloc is empty \n");
    }
    else
    {
        (*((size_t *)ptr - 1)) <<= 1;
    }
}

// Second part version
void myfree_v2(void *ptr)
{
    if (!((size_t)small_tab <= (size_t)ptr && (size_t)ptr <= (size_t)(small_tab + 128 * MAX_SMALL)))
        printf("ERROR : the pointer given isn't accessible by myfree \n");

    else if (((size_t)ptr - (size_t)small_tab) % 128 != sizeof(size_t))
    {
        printf("ERROR : the pointer given is not at the begining of a bloc \n");
    }
    else if (*((size_t *)ptr - 1) % 2 == 0)
    {
        printf("ERROR : the bloc is empty \n");
    }
    else
    {
        char **bloc_ptr = (char **)((char *)ptr - sizeof(size_t));
        // The freed bloc points to fst_free
        *bloc_ptr = small_free;
        // fst_free points to the freed bloc
        small_free = (char *)ptr - sizeof(size_t);
    }
}

// Final version
void myfree(void *ptr)
{
    // Checks if ptr points to a small bloc
    if ((size_t)small_tab <= (size_t)ptr && (size_t)ptr <= (size_t)(small_tab + 128 * MAX_SMALL))
    {
        if (((size_t)ptr - (size_t)small_tab) % 128 != sizeof(size_t))
        {
            printf("ERROR : the pointer given is not at the begining of a bloc \n");
        }
        else if (*((size_t *)ptr - 1) % 2 == 0)
        {
            printf("ERROR : the bloc is already free \n");
        }
        else
        {
            char **bloc_ptr = (char **)((char *)ptr - sizeof(size_t));
            // The freed bloc points to fst_free
            *bloc_ptr = small_free;
            // fst_free points to the freed bloc
            small_free = (char *)ptr - sizeof(size_t);
        }
    }
    else
    {
        size_t *ptr_sizet = (size_t *)ptr - 2;
        if (*(ptr_sizet) % 2 == 0)
        {
            printf("ERROR : the bloc is already free \n");
        }
        else
        {
            // Puts the bloc at the begining of the free bloc list
            *(size_t **)(ptr_sizet) = big_free;
            big_free = ptr_sizet;
        }
    }
}

// Copies the size firsts characters of ptr1 in ptr2
void copy(char *ptr1, char *ptr2, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        *(ptr2 + i) = *(ptr1 + i);
    }
}

// First and second part version
void *myrealloc_v1(void *ptr, size_t size)
{
    if (!((size_t)small_tab <= (size_t)ptr && (size_t)ptr <= (size_t)(small_tab + 128 * MAX_SMALL)))
    {
        printf("ERROR : the pointer given isn't accessible by myrealloc \n");
    }
    else if (((size_t)ptr - (size_t)small_tab) % 128 != sizeof(size_t))
    {
        printf("ERROR : the pointer given is not at the begining of a bloc \n");
    }
    else if (size > SIZE_BLK_SMALL || *((size_t *)ptr - 1) % 2 == 0)
    {
        return NULL;
    }
    return ptr;
}

// Final version
void *myrealloc(void *ptr, size_t size)
{
    size_t bloc_size;
    if ((size_t)small_tab <= (size_t)ptr && (size_t)ptr <= (size_t)(small_tab + 128 * MAX_SMALL))
    {
        if (((size_t)ptr - (size_t)small_tab) % 128 != sizeof(size_t))
        {
            printf("ERROR : the pointer given is not at the begining of a bloc \n");
            return NULL;
        }
        bloc_size = SIZE_BLK_SMALL;
    }
    else
    {
        size_t *ptr_sizet = (size_t *)ptr;
        if (*(ptr_sizet - 2) % 2 == 0)
        {
            printf("ERROR : the bloc is free");
            return NULL;
        }
        bloc_size = *(ptr_sizet - 1);
    }
    if (size <= bloc_size)
    {
        return ptr;
    }
    void *new_ptr = mymalloc(size);
    copy(ptr, new_ptr, bloc_size);
    myfree(ptr);
    return new_ptr;
}

/******************************* FUNCTIONS TO VISUALIZE MEMORY *******************************/

void ctrl_read(void *ptr, size_t offset, size_t size);

void print_bloc(void *ptr)
{
    /*Displays informations of bloc which writing zone is at adress ptr*/
    size_t free_mark;
    size_t bloc_size;
    size_t *bloc_ptr;
    if ((size_t)small_tab <= (size_t)ptr && (size_t)ptr <= (size_t)(small_tab + 128 * MAX_SMALL))
    {
        if (((size_t)ptr - (size_t)small_tab) % 128 != sizeof(size_t))
        {
            printf("ERROR : the pointer given is not at the begining of a wrinting zone \n");
            exit(1);
        }
        free_mark = *((size_t *)ptr - 1);
        bloc_size = SIZE_BLK_SMALL;
        bloc_ptr = (size_t *)ptr - 1;
    }
    else
    {
        free_mark = *((size_t *)ptr - 2);
        bloc_size = *((size_t *)ptr - 1) - 2 * sizeof(size_t);
        bloc_ptr = (size_t *)ptr - 2;
    }
    printf("____________________________ DISPLAYING BLOC : %p ____________________________  \n", (void *)bloc_ptr);
    if (free_mark % 2 == 0)
        printf("Type : Free \n");
    else
        printf("Type : Occupied \n");
    printf("Begining of writing zone : %p \n", (void *)ptr);
    printf("Size of writing zone : %lu \n", bloc_size);
    printf("Content : \n");
    if (free_mark % 2 == 0)
        printf("Unreadable (free) \n");
    else 
        ctrl_read(ptr, 0, bloc_size);
}

void print_freeList()
{
    /* Displays the list of free blocs in memory */
    printf("************************************************ \n");
    printf("DISPLAYING THE LIST OF FREE BLOCS IN MEMORY \n\n");
    printf("LIST OF SMALL FREE BLOCS IN MEMORY : \n");
    size_t index;
    size_t *ptr_small = (size_t *)small_free;
    while (ptr_small != NULL)
    {
        index = ((size_t)ptr_small - (size_t)small_tab) / 128;
        printf("%p (index : %lu, next : %p); ", (void *)ptr_small, index, (void *)*(size_t **)ptr_small);
        ptr_small = *(size_t **)ptr_small;
    }
    printf("\n\n");
    printf("LIST OF LARGE FREE BLOCS IN MEMORY : \n");
    size_t *ptr_large = big_free;
    while (ptr_large != NULL)
    {
        printf("%p (size : %lu, next : %p); ", (void *)ptr_large, *(ptr_large + 1), (void *)*(size_t **)ptr_large);
        ptr_large = *(size_t **)ptr_large;
    }
    printf("\n");
    printf("************************************************ \n");
}

void print_smallOccupiedList()
{
    /*Displays the occupied blocs and returns an array of free blocs*/
    printf("************************************************ \n");
    printf("DISPLAYING SMALL OCCUPIED BLOCS : \n");
    int counter = 0;
    for (size_t b = 0; b < MAX_SMALL; b++)
    {
        if (*(size_t *)(small_tab + 128 * b) % 2 == 1)
        {
            printf("%lu ; ", b);
            counter++;
        }
    }
    printf("\n");
    printf("************************************************ \n");
}

void print_mem()
{
    /*Displays the list of free blocs and small occupied blocs*/
    printf("MEMORY STATE : \n");
    print_freeList();
    print_smallOccupiedList();
}

/******************************* CONTROLLED READING/WRITING FUNCTIONS *******************************/

void ctrl_read(void *ptr, size_t offset, size_t size)
{
    /* Reads the characters at positions [offset, offset + size - 1] in the bloc which writng zone begins at ptr
    Must check that :
    #1 ptr is indeed at the begining of a writing zone (if possible)
    #2 The bloc is occupied
    #3 [offset, offset + size - 1] is include in the bloc
    */
    size_t free_mark;
    size_t bloc_size;
    if ((size_t)small_tab <= (size_t)ptr && (size_t)ptr <= (size_t)(small_tab + 128 * MAX_SMALL))
    {
        // Checks #1
        if (((size_t)ptr - (size_t)small_tab) % 128 != sizeof(size_t))
        {
            printf("ERROR : the pointer given is not at the begining of a writing zone \n");
            exit(1);
        }
        free_mark = *((size_t *)ptr - 1);
        bloc_size = SIZE_BLK_SMALL;
    }
    else
    {
        free_mark = *((size_t *)ptr - 2);
        bloc_size = *((size_t *)ptr - 1);
    }
    // Checks #2
    if (free_mark % 2 == 0)
    {
        printf("ERROR : Can't read, the bloc is free \n");
        exit(1);
    }
    // Checks #3
    if (offset + size > bloc_size)
    {
        printf("ERROR : Trying to read to much data \n");
        exit(1);
    }
    for (size_t i = offset; i < offset + size; i++)
    {
        printf("%d", ((char *)ptr)[i]);
    }
    printf("\n");
}

void ctrl_write(void *ptr, size_t offset, size_t size, char *src)
{
    /* Writes the size first characters of src at the position [offset, offset + size - 1] in the bloc which writng zone begins at ptr
    Must check that :
    #1 ptr is indeed at the begining of a writing zone (if possible)
    #2 The bloc is occupied
    #3 [offset, offset + size - 1] is include in the bloc
    */
    size_t free_mark;
    size_t bloc_size;
    if ((size_t)small_tab <= (size_t)ptr && (size_t)ptr <= (size_t)(small_tab + 128 * MAX_SMALL))
    {
        // Checks #1
        if (((size_t)ptr - (size_t)small_tab) % 128 != sizeof(size_t))
        {
            printf("ERROR : the pointer given is not at the begining of a wrinting zone \n");
            exit(1);
        }
        free_mark = *((size_t *)ptr - 1);
        bloc_size = SIZE_BLK_SMALL;
    }
    else
    {
        free_mark = *((size_t *)ptr - 2);
        bloc_size = *((size_t *)ptr - 1);
    }
    // Checks #2
    if (free_mark % 2 == 0)
    {
        printf("ERROR : Can't write, the bloc is free \n");
        exit(1);
    }
    // Checks #3
    if (offset + size > bloc_size)
    {
        printf("ERROR : Trying to write to much data \n");
        exit(1);
    }
    copy(src, (char *)ptr + offset, size);
}
