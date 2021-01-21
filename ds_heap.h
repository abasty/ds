/*
 * Copyright © 2021 Alain Basty
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __DS_HEAP_H__
#define __DS_HEAP_H__

#include <stddef.h>

#include "ds_common.h"
#include "ds_lifo.h"

typedef struct ds_heap_s ds_heap_t;
struct ds_heap_s
{
    ds_lifo_t free_list;
    void *store;
};

/**
 \* @brief Initialize a heap given an array of typed elements
 *
 \* @param _heap The heap to initialize
 \* @param _store An array, possibly calloc() allocated, of _type_t elements
 \* @param _nmemb Number of elements in the array
 \* @param _type_t Type of elements
 */
#define DS_HEAP_INIT(_heap, _store, _nmemb, _type_t) \
    do                                               \
    {                                                \
        _heap.store = _store;                        \
        ds_lifo_t *free_list = &(_heap.free_list);   \
        _type_t *item = _heap.store;                 \
        ds_lifo_init(free_list, 0);                  \
        for (size_t i = 0; i < (_nmemb); i++)        \
        {                                            \
            ds_lifo_push(free_list, item);           \
            item++;                                  \
        }                                            \
    } while (0)

/**
 \* @brief Take an element from the heap free list
 *
 \* @param heap The heap to take the element from
 \* @return The taken element or 0 if free list is empty
 */
static inline void *ds_heap_alloc(ds_heap_t *heap)
{
    return ds_lifo_pop(&heap->free_list);
}

/**
 \* @brief Give back an element to the heap free list
 *
 \* @param heap The heap to give back the element to
 \* @param item The element to give back to the heap
 */
static inline void ds_heap_free(ds_heap_t *heap, void *item)
{
    ds_lifo_push(&heap->free_list, item);
}

#endif // __DS_HEAP_H__
