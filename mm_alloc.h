/*
 * mm_alloc.h
 *
 * A clone of the interface documented in "man 3 malloc".
 */

#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct block_meta
{
    size_t size;
    bool free;
    struct block_meta *next;
    struct block_meta *prev;
    char allocated_memory[0];
} block_meta;

/* Start empty list */
static block_meta head;

/* Start free list */
static block_meta *free_list; 

block_meta* coalesce(block_meta* block_meta_ptr,block_meta* p);
block_meta *Extend_Memory(size_t meta_number);
size_t calculate_meta_num(size_t size);

void *mm_malloc(size_t size);
void *mm_realloc(void *ptr, size_t size);
void mm_free(void *ptr);
