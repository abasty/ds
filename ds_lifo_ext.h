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

#ifndef __DS_LIFO_EXT_H__
#define __DS_LIFO_EXT_H__

#include <stddef.h>

#include "ds_common.h"

typedef struct ds_lifo_ext_item_s ds_lifo_ext_item_t;
struct ds_lifo_ext_item_s
{
    ds_lifo_ext_item_t *next;
    void *object;
};

typedef struct ds_lifo_ext_s ds_lifo_ext_t;
struct ds_lifo_ext_s
{
    size_t count;
    ds_lifo_ext_item_t *root;
};

static inline void ds_lifo_ext_init(ds_lifo_ext_t *lifo)
{
    lifo->root = 0;
    lifo->count = 0;
}

static inline void ds_lifo_ext_push(ds_lifo_ext_t *lifo, ds_lifo_ext_item_t *item, void *object)
{
    lifo->count++;
    item->object = object;
    item->next = lifo->root;
    lifo->root = item;
}

static inline void *ds_lifo_ext_pop(ds_lifo_ext_t *lifo)
{
    ds_lifo_ext_item_t *item = lifo->root;
    if (!item)
        return 0;
    lifo->root = item->next;
    item->next = 0;
    lifo->count--;
    return item->object;
}

#endif // __DS_LIFO_EXT_H__
