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
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */

#include "mm_alloc.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//zero-fill parameter
int z_f = 0;

/*
memory allocation function. 
allocates blocks on the heap memory.
*/

void appropriate_message(char *str, void *ptr, size_t size)
{

    if (!ptr)
    {
        fprintf(stderr, "failed to %s %ld bytes\n", str, size * sizeof(char));
        return;
    }
    printf("%sd %ld bytes successfully\n", str, size * sizeof(char));
}

void *mm_malloc(size_t size)
{

    //number of metadata
    size_t num_m = calculate_meta_num(size);

    //return NULL if passed 0 as allocation size
    if (size == 0)
    {
        appropriate_message("Allocate", (void *)NULL, size);
        return NULL;
    }

    //struct block_meta parameters
    block_meta *blk;
    block_meta *previous_blk;

    previous_blk = free_list;

    // initialize the block_meta head if previous_blk == NULL
    if (previous_blk == NULL)
    {
        head.size = 0;
        free_list = &head;
        head.prev = &head;
        head.next = free_list;
        previous_blk = &head;
    }

    //blk gets the next address of blk_perv address
    blk = previous_blk->next;

    //itirating over the heap as first step of first fit algorithm
    while (true)
    {
        if (blk->size >= num_m)
        {
            // big enough memory space is found

            if (blk->size > num_m)
            {
                //too large chunk.
                blk->size = blk->size - num_m;
                blk = blk + blk->size;
                blk->size = num_m;
            }
            else
            {
                //fits exactly
                previous_blk->next = blk->next;
                blk->next->prev = previous_blk;
            }

            //change free to false
            blk->free = false;
            free_list = previous_blk;
            //zero-fill the allocated memory before returning its address
            memset(blk->allocated_memory, z_f, (blk->size * sizeof(block_meta) - sizeof(block_meta)));
            //return allocated memory pointer
            appropriate_message("Allocate", (void *)blk->allocated_memory, size);
            return (void *)(blk->allocated_memory);
        }

        //create more space on the heap
        if (blk == free_list)
        {
            blk = Extend_Memory(num_m);
            if (blk)
                continue;
            else
                // return NULL when can't allocate anymore heap memory
                appropriate_message("Allocate", (void *)NULL, size);
            return NULL;
        }
        //update the pointers distination
        previous_blk = blk, blk = blk->next;
    }
}

/*
free memory allocation function. 
frees the allocated blocks on the heap memory.
*/
void mm_free(void *ptr)
{
    //check if NULL -> do nothing
    if (!ptr)
    {
        return;
    }
    //struct block_meta parameters
    block_meta *block_meta_ptr, *b_p;

    //initializing the block_meta_ptr
    block_meta_ptr = (block_meta *)ptr - 1;

    //convert the block's property free, from false to true
    block_meta_ptr->free = true;

    b_p = free_list;

    int check = block_meta_ptr->size * sizeof(block_meta) - sizeof(block_meta);
    if (check < 0)
    {
        fprintf(stderr, "mm_free(): invalid pointer\n");
        //in real free it should do abort();
        return;
    }
    //zero-filling it
    memset(block_meta_ptr->allocated_memory, z_f, check);

    //check if coalesce should be done
    free_list = coalesce(block_meta_ptr, b_p);
}

/*
reallocating memory function, 
minimize or extend the chunk of memory
*/
void *mm_realloc(void *ptr, size_t size)
{
    //Handle the cases of realloc:
    //check 1
    if (ptr && size == 0)
    {
        return mm_malloc(0);
    }
    //check 2
    if (!ptr && size > 0)
    {
        return mm_malloc(size);
    }
    //check 3
    if (!ptr && size == 0)
    {
        mm_free(ptr);
        return NULL;
    }

    size_t num_m;
    //calculate number of meta data
    num_m = calculate_meta_num(size);

    block_meta *block_meta_ptr;
    block_meta_ptr = (block_meta *)ptr - 1;

    //check if we dont have  enough space(size big enough)
    if (block_meta_ptr->size < num_m)
    {
        //calls the mm_malloc function
        void *new_ptr = (void *)mm_malloc(size);

        //if cannot create allocated memory return NULL
        //not modify the original block
        if (new_ptr == NULL)
            return NULL;

        //size of data to be copy
        size_t size_to_copy = (block_meta_ptr->size - 1) * sizeof(block_meta);

        //copy the data to new_ptr in size_to_copy size
        memcpy(new_ptr, block_meta_ptr->allocated_memory, size_to_copy);

        //free the pointer
        mm_free(ptr);

        //zero-filling
        memset((char *)new_ptr + size_to_copy, z_f, size - size_to_copy);

        //return pointer
        return new_ptr;
    }

    //don’t need to move the block
    else if (block_meta_ptr->size >= num_m)
    {
        return ptr;
    }

    return NULL;
}

//coalescing function : called when there is a need to coalesce free chunks of memory
block_meta *coalesce(block_meta *block_meta_ptr, block_meta *ptr)
{
    //loop to find the address where coalesce is need and break when u find it
    while (block_meta_ptr <= ptr || block_meta_ptr >= ptr->next)
    {
        if (!(ptr < ptr->next))
        {
            if (!(block_meta_ptr <= ptr))
            {
                if ((block_meta_ptr >= ptr->next))
                {
                    break;
                }
            }
        }

        ptr = ptr->next;
    }

    //if the block_meta_ptr address +  its size, not getting to the next address of p
    if (!(block_meta_ptr + block_meta_ptr->size == ptr->next))
    {

        block_meta_ptr->next = ptr->next;
        block_meta_ptr->next->prev = block_meta_ptr;
    }
    else
    {

        block_meta_ptr->size = block_meta_ptr->size + ptr->next->size;
        block_meta_ptr->next = ptr->next->next;
        block_meta_ptr->next->prev = block_meta_ptr;
    }

    //if the p address +  its size, not getting to the  address of block_meta_ptr
    if (!(ptr + ptr->size == block_meta_ptr))
    {

        ptr->next = block_meta_ptr;
        block_meta_ptr->prev = ptr;
    }
    else
    {
        ptr->size = ptr->size + block_meta_ptr->size;
        ptr->next = block_meta_ptr->next;
        block_meta_ptr->prev = ptr;
    }

    return ptr;
}

//extending memory function : called when there is a need to extend memory
block_meta *Extend_Memory(size_t meta_number)
{

    //parameters
    void *extend;
    block_meta *blk;
    //POSIX interface PAGESIZE- Size of a page in bytes.
    //old function: getpagesize()
    //new function: sysconf(_SC_PAGESIZE)
    const size_t p_size = sysconf(_SC_PAGESIZE);
    size_t memory;

    /*
    check if the meta_number is bigger or less than size of a page size in bytes, 
    and initialize the memory parameter respectively.
    */
    if (meta_number * sizeof(block_meta) >= p_size)
    {
        memory = -2 * p_size + sizeof(block_meta) * meta_number;
    }
    else
    {
        memory = p_size;
    }

    //moving forward the break point by calculated needed memory size
    extend = sbrk(memory);

    //check if it can extend
    if (extend == (void *)-1)
        return NULL;

    blk = (block_meta *)extend;

    blk->size = memory / sizeof(block_meta);

    //free the blk
    mm_free(blk->allocated_memory);
    return free_list;
}

size_t calculate_meta_num(size_t size)
{
    return (size + 2 * sizeof(block_meta)) / sizeof(block_meta) - (1 / (sizeof(block_meta)));
}
