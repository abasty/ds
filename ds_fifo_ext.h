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

#ifndef __DS_FIFO_EXT_H__
#define __DS_FIFO_EXT_H__

#include <stddef.h>

#include "ds_common.h"

typedef struct ds_fifo_ext_item_s ds_fifo_ext_item_t;
struct ds_fifo_ext_item_s
{
    ds_fifo_ext_item_t *next;
    void *object;
};

typedef struct ds_fifo_ext_s ds_fifo_ext_t;
struct ds_fifo_ext_s
{
    size_t count;
    ds_fifo_ext_item_t *root;
    ds_fifo_ext_item_t *last;
};

static inline void ds_fifo_ext_init(ds_fifo_ext_t *fifo)
{
    fifo->root = 0;
    fifo->last = 0;
    fifo->count = 0;
}

static inline void ds_fifo_ext_enq(ds_fifo_ext_t *fifo, ds_fifo_ext_item_t *item, void *object)
{
    fifo->count++;
    item->object = object;
    item->next = 0;
    if (fifo->last)
        fifo->last->next = item;
    else
        fifo->root = item;
    fifo->last = item;
}

static inline void *ds_fifo_ext_deq(ds_fifo_ext_t *fifo)
{
    ds_fifo_ext_item_t *item = fifo->root;
    if (!item)
        return 0;
    fifo->root = item->next;
    if (!fifo->root)
        fifo->last = 0;
    fifo->count--;
    return item->object;
}

#endif // __DS_FIFO_EXT_H__
