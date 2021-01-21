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

#ifndef __DS_FIFO_H__
#define __DS_FIFO_H__

#include <stddef.h>

#include "ds_common.h"

typedef struct ds_fifo_item_s ds_fifo_item_t;
struct ds_fifo_item_s
{
    ds_fifo_item_t *next;
};

typedef struct ds_fifo_s ds_fifo_t;
struct ds_fifo_s
{
    size_t count;
    ds_fifo_item_t *root;
    size_t _offset_in_object;
    ds_fifo_item_t *last;
};

static inline void ds_fifo_init(ds_fifo_t *fifo, size_t offset_in_object)
{
    fifo->_offset_in_object = offset_in_object;
    fifo->root = 0;
    fifo->last = 0;
    fifo->count = 0;
}

static inline void ds_fifo_enq(ds_fifo_t *fifo, void *object)
{
    ds_fifo_item_t *item = DS_ITEM_OF(fifo, object);
    fifo->count++;
    item->next = 0;
    if (fifo->last)
        fifo->last->next = item;
    else
        fifo->root = item;
    fifo->last = item;
}

static inline void *ds_fifo_deq(ds_fifo_t *fifo)
{
    ds_fifo_item_t *item = fifo->root;
    if (!item)
        return 0;
    fifo->root = item->next;
    if (!fifo->root)
        fifo->last = 0;
    fifo->count--;
    return DS_OBJECT_OF(fifo, item);
}

#endif // __DS_FIFO_H__
