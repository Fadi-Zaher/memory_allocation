/* Assignment 5
 * Operating Systems
 * Kinneret College on the Sea of Galilee
 * Semester 1 5781
 * 
 * Submitted by: 
 * 
 * Fadi Zaher - 205792526
 * Niv Sahar - 205808272
 * 
 * mm_test.c
 *
 * Testing file for memory allocation and deallocation.
 */

#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "mm_alloc.h"
#include <unistd.h>
#include <sys/resource.h>

//declartion on the functions
void print_content(int num, void *ptr);
void *allocation_int(int num, int s);
void *allocation_char(int num, int s);
void myTests();

/* 
Test task 1:
(a) Allocates 100 bytes
(b) Edits a few bytes
(c) free the allocation
(d) Allocates 0 bytes
(e) free a non-existent pointer
(f) reallocates from 0
(g) reallocates to 0
*/
void basic()
{

    /*  (a) Allocates 100 bytes   */
    printf("\n\e[1;91m---------------- Basic Tests ----------------\033[0m\n\n");
    int size1 = 100;
    int size2 = 50;
    char *block_ptr1 = (char *)mm_malloc(size1 * sizeof(char));
    printf("\n\e[1;93mallocated 100 bytes successfully\033[0m\n");
    printf("Allocated memory: %p\n", block_ptr1);

    /*    (b) Edits a few bytes    */
    block_ptr1[0] = 'a';
    block_ptr1[1] = 'b';
    block_ptr1[2] = 'c';
    printf("\n\e[1;93mEdits few bytes succesfuly\033[0m");
    printf("\n%s", block_ptr1);

    /*    (c) frees the allocation    */
    printf("\n\e[1;93mfreed %ld Bytes\033[0m\n", size1 * sizeof(char));
    mm_free(block_ptr1);

    /*    (d) Allocates 0 bytes    */
    printf("\n\e[1;93mBlock2 - Allocates 0 bytes\033[0m\n\n");
    char *block_ptr2 = mm_malloc(0);
    printf("Mm_malloc 0 size give a: %s", block_ptr2);

    /*    (e) frees a non-existent pointer    */
    printf("\n\e[1;93mfree non-existent pointer\033[0m\n\n");
    void *block_ptr3 = NULL;
    mm_free(block_ptr3);

    /*    (f) Mm_reallocates from 0    */
    void *block_ptr4 = (void *)mm_malloc(0);
    block_ptr4 = mm_realloc(block_ptr4, size2 * sizeof(int));
    printf("\n\e[1;93mReallocated 0 bytes to %ld bytes successfully\033[0m\n\n", size2 * sizeof(int));
    printf("%p\n", block_ptr4);

    /*    (g) Mm_reallocates to 0    */
    printf("\n\e[1;93mReallocates to 0\033[0m\n\n");
    block_ptr4 = mm_realloc(block_ptr4, 0);
    printf("\n\e[6;95mFirst Test Finished\033[0m\n\n");
}

/*
Test task 2:
(a) 10 allocataions
(b) 5 frees
(c) 10 mm_reallocations
(d) 5 frees
(e) 10 allocations

After each allocation, perform reads and writes to the newly allocated space. After
reallocation, check that contents are preserved.

*/
void secondTest()
{
    /* Do more tests here (see PDF). */
    printf("\n\e[1;91m---------------- Second Tests ---------------\033[0m\n\n");

    printf("\nReading and writing to allocated space\n");

    //10 Allocations!

    void *int_blocks[5];
    void *char_blocks[5];

    for (int i = 0; i < 5; i++)
    {
        //Function that calls the mm_malloc
        int_blocks[i] = allocation_int(2 * i + 1, 100);
        if (int_blocks[i] == NULL)
        {
            fprintf(stderr, "%s", "Allocation failed!\n");
        }
        //Function that calls the mm_malloc
        char_blocks[i] = allocation_char(i * 2 + 2, 200);
        if (char_blocks[i] == NULL)
        {
            fprintf(stderr, "%s", "Allocation failed!\n");
        }
    }
    //#################################################
    //MM_FREES
    mm_free(int_blocks[0]);
    mm_free(int_blocks[1]);
    mm_free(int_blocks[2]);
    mm_free(char_blocks[0]);
    mm_free(char_blocks[1]);

    printf("\n\e[1;94mMm_freed 5 block succesfully!\033[0m\n");
    int_blocks[0] = allocation_int(1, 19 * sizeof(int));
    char_blocks[0] = allocation_char(2, 30 * sizeof(char));
    int_blocks[1] = allocation_int(3, 30 * sizeof(int));
    char_blocks[1] = allocation_char(4, 50 * sizeof(char));
    int_blocks[2] = allocation_int(5, 10 * sizeof(int));

    //reallocate the free bytes!
    printf("\n\e[1;94mallocated again the 5freed blocks!\033[0m\n");

    printf("\n\e[1;94mreallocated 10 blocks and print content\033[0m\n");
    for (int i = 0; i < 5; i++)
    {
        printf("\n\e[1;91mBlock %d\033[0m\n", 2 * i + 1);
        int size1 = 25;
        int *ptr1 = (int *)mm_realloc(int_blocks[i], size1 * sizeof(int));
        if (int_blocks[i] == NULL)
        {
            fprintf(stderr, "%s", "reallocation failed!\n");
        }
        for (int i = 0; i < size1; i++)
        {
            printf("%d, ", ptr1[i]);
        }

        printf("\n\e[1;91mBlock %d\033[0m\n", 2 * i + 2);
        int size2 = 100;
        char *ptr2 = (char *)mm_realloc(char_blocks[i], size2 * sizeof(char));
        printf("%s", ptr2);
    }

    printf("\n");

    //####################################
    //MM_FREES
    mm_free(int_blocks[1]);
    mm_free(int_blocks[2]);
    mm_free(int_blocks[3]);
    mm_free(char_blocks[3]);
    mm_free(char_blocks[4]);

    printf("\n\e[1;94mfreed 5 block succesfully!\033[0m\n");

    //####################################
    //Allocate again the free block before make mm_reallocation
    int_blocks[1] = allocation_int(1, 19 * sizeof(int));
    int_blocks[2] = allocation_char(2, 10 * sizeof(int));
    int_blocks[3] = allocation_int(3, 14 * sizeof(int));
    char_blocks[3] = allocation_char(4, 30 * sizeof(char));
    char_blocks[4] = allocation_int(5, 14 * sizeof(char));

    //#####################################
    //More 10 allocations

    for (int i = 0; i < 5; i++)
    {
        //Function that calls the mm_malloc
        int_blocks[i] = allocation_int(2 * i + 1, 15 * sizeof(int));
        if (int_blocks[i] == NULL)
        {
            fprintf(stderr, "%s", "Allocation failed!\n");
        }
        //Function that calls the mm_malloc
        char_blocks[i] = allocation_char(i * 2 + 2, 20 * sizeof(char));
        if (char_blocks[i] == NULL)
        {
            fprintf(stderr, "%s", "Allocation failed!\n");
        }
    }
    printf("\n");
    printf("\n\e[1;95mSecond Test Finished\033[0m\n\n");
}

/*
Test task 3:
(a) 20 allocataions
(b) 5 mm_frees
(c) 20 allocations
(d) 20 mm_reallocations
(e) 15 mm_frees
(f) 10 allocations
(i) 10 mm_reallocations

After each allocation, perform reads and writes to the newly allocated space. After
mm_reallocation, check that contents are preserved.

*/
void thirdTest()
{

    printf("\n\e[1;91m---------------- Third Test ---------------\033[0m\n\n");
    /*    (a) 20 allocataions    */
    void *int_blocks[10];
    void *char_blocks[10];

    for (int i = 0; i < 10; i++)
    {
        //Function that calls the mm_malloc
        int_blocks[i] = allocation_int(2 * i + 1, 6 * sizeof(int));
        if (int_blocks[i] == NULL)
            fprintf(stderr, "%s", "Allocation failed!\n");

        //Function that calls the mm_malloc
        char_blocks[i] = allocation_char(i * 2 + 2, 10 * sizeof(char));
        if (char_blocks[i] == NULL)
            fprintf(stderr, "%s", "Allocation failed!\n");
    }

    /*    (b) 5 mm_frees    */
    mm_free(int_blocks[1]);
    mm_free(int_blocks[2]);
    mm_free(int_blocks[3]);
    mm_free(char_blocks[3]);
    mm_free(char_blocks[4]);

    printf("\n\n\e[1;94mFreed 5 blocks succesfully!\033[0m\n\n");

    /*  (c) 20 allocations   */
    printf("\n\e[1;94Mallocate more 20 blocks\033[0m\n");
    void *int_blocks_new[10];
    void *char_blocks_new[10];

    for (int i = 0; i < 10; i++)
    {
        //Function that calls the mm_malloc
        int_blocks_new[i] = allocation_int(2 * i + 1, 35 * sizeof(int));
        if (int_blocks_new[i] == NULL)
            fprintf(stderr, "%s", "Allocation failed!\n");

        //Function that calls the mm_malloc
        char_blocks_new[i] = allocation_char(i * 2 + 2, 30 * sizeof(char));
        if (char_blocks_new[i] == NULL)
            fprintf(stderr, "%s", "Allocation failed!\n");
    }
    /*    (d) 20 mm_reallocations    */
    printf("\n\e[1;94mReallocated 10 blocks and print content\033[0m\n");
    for (int i = 0; i < 10; i++)
    {
        printf("\n\e[1;91mBlock %d\033[0m\n", 2 * i + 1);
        int size1 = 10;
        int *ptr1 = (int *)mm_realloc(int_blocks_new[i], size1 * sizeof(int));
        if (int_blocks_new[i] == NULL)
            fprintf(stderr, "%s", "Reallocation failed!\n");

        for (int i = 0; i < size1; i++)
        {
            printf("%d, ", ptr1[i]);
        }

        printf("\n\e[1;91mBlock %d\033[0m\n", 2 * i + 2);
        int size2 = 7;
        char *ptr2 = (char *)mm_realloc(char_blocks_new[i], size2 * sizeof(char));
        printf("%s", ptr2);
    }

    printf("\n");

    /*    (e) 15 mm_frees    */
    for (int i = 0; i < 8; i++)
        mm_free(int_blocks_new[i]);

    for (int j = 0; j < 7; j++)
        mm_free(char_blocks_new[j]);

    /*    (f) 10 allocations    */
    int *int_blocks_new1[5];
    char *char_blocks_new1[5];

    printf("\n\e[1;94mallocate more 10 blocks\033[0m\n");

    for (int i = 0; i < 5; i++)
    {
        //Function that calls the mm_malloc
        int_blocks_new1[i] = allocation_int(2 * i + 1, 15 * sizeof(int));
        if (int_blocks_new1[i] == NULL)
        {
            fprintf(stderr, "%s", "Allocation failed!\n");
        }
        //Function that calls the mm_malloc
        char_blocks_new1[i] = allocation_char(i * 2 + 2, 10 * sizeof(char));
        if (char_blocks_new1[i] == NULL)
        {
            fprintf(stderr, "%s", "Allocation failed!\n");
        }
    }

    /*    (i) 10 allocations    */
    printf("\n\e[1;94mReallocated 10 blocks and print content\033[0m\n");
    for (int i = 0; i < 5; i++)
    {
        printf("\n\e[1;91mBlock %d\033[0m\n", 2 * i + 1);
        int size1 = 12;
        int *ptr1 = (int *)mm_realloc(int_blocks_new1[i], size1 * sizeof(int));
        if (int_blocks_new1[i] == NULL)
            fprintf(stderr, "%s", "Reallocation failed!\n");

        for (int i = 0; i < size1; i++)
        {
            printf("%d, ", ptr1[i]);
        }

        printf("\n\e[1;91mBlock %d\033[0m\n", 2 * i + 2);
        int size2 = 10;
        char *ptr2 = (char *)mm_realloc(char_blocks_new1[i], size2 * sizeof(char));
        printf("%s", ptr2);
    }

    printf("\n");
    printf("\n\e[6;95mThird Test Finished\033[0m\n\n");
}

//Own tests for the program
void myTests()
{

    printf("\n\e[1;91m----------------------More Tests!----------------------\033[0m\n");
    int *arrays[10];
    int i, j, k;

    for (k = 0; k < 1; k++)
    {
        printf("\n\e[4;92mBefore mm_frees\033[0m\n");
        for (i = 0; i < 10; i++)
        {
            arrays[i] = (int *)mm_malloc(sizeof(int) * (i + 1));
            for (j = 0; j <= i; j++)
            {
                arrays[i][j] = j;
                printf("%d, ", arrays[i][j]);
            }
            printf("\n");
        }

        for (i = 0; i < 10; i++)
        {

            mm_free(arrays[i]);
        }
        printf("\n\e[1;92mFinished 10 allocations and after 10 frees!\033[0m\n");

        printf("\n\e[4;92mAfter frees\033[0m\n");

        for (i = 0; i < 10; i++)
        {
            for (j = 0; j <= i; j++)
            {

                printf("%d, ", arrays[i][j]);
            }
            printf("\n");
        }
    }

    printf("\n\n");

    int *ptr = (int *)mm_malloc(sizeof(int) * 2);
    if (ptr)
        printf("Allocate %ld OK\n", sizeof(int) * 2);
    else
    {
        printf("Allocate %ld FAILED\n", sizeof(int) * 2);
    }

    int *ptr_new;

    *ptr = 10;
    *(ptr + 1) = 20;

    ptr_new = (int *)mm_realloc(ptr, sizeof(int) * 3);

    if (ptr_new)
        printf("Reallocate %ld OK\n", sizeof(int) * 3);
    else
    {
        printf("Reallocate %ld FAILED\n", sizeof(int) * 2);
    }
    printf("Initialize the array\n");
    *(ptr_new + 2) = 30;
    for (i = 0; i < 3; i++)
        printf("%d ", *(ptr_new + i));

    printf("\n");
}

//Allocation for int and make reading and writing to memory
void *allocation_int(int num, int s)
{
    int *ptr = NULL;
    printf("\n\e[1;93mBlock %d reading and writing\033[0m\n", num);
    ptr = (int *)mm_malloc(s * sizeof(int));
    if (!ptr)
    {
        return NULL;
    }
    printf("Allocated memory: %p\ncontent:\n", ptr);

    for (int i = 0; i < s / (sizeof(int)); i++)
    {
        ptr[i] = 2 + rand() % 7;
        printf("%d ,", ptr[i]);
    }
    printf("\n");
    return ptr;
}

//Allocation for char and make reading and writing to memory
void *allocation_char(int num, int s)
{
    char *ptr = NULL;
    printf("\n\e[1;93mBlock %d reading and writing\033[0m\n", num);
    ptr = (char *)mm_malloc(s * sizeof(char));
    if (!ptr)
    {
        return NULL;
    }
    printf("Allocated memory: %p\ncontent:\n", ptr);

    for (int i = 0; i < s; i++)
    {
        *(ptr + i) = 'a' + rand() % 11;
    }
    printf("%s", ptr);
    printf("\n");

    return ptr;
}

/*
Main funtction that runs all the tests
3 given assignment test and one more private test
*/
int main()
{
    basic();
    secondTest();
    thirdTest();
    myTests();
    return 0;
}
