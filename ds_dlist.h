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

#ifndef __DS_DLIST_H__
#define __DS_DLIST_H__

#include <stddef.h>

#include "ds_common.h"

typedef struct ds_dlist_item_s ds_dlist_item_t;
struct ds_dlist_item_s
{
    ds_dlist_item_t *next;
    ds_dlist_item_t *prev;
};

typedef struct ds_dlist_s ds_dlist_t;
struct ds_dlist_s
{
    size_t count;
    ds_dlist_item_t *root;
    size_t _offset_in_object;
    ds_dlist_item_t *last;
};

static inline void ds_dlist_init(ds_dlist_t *dlist, size_t offset_in_object)
{
    dlist->_offset_in_object = offset_in_object;
    dlist->root = 0;
    dlist->last = 0;
    dlist->count = 0;
}

/**
 * @brief Add an item at the end of the list
 *
 * @param dlist The list
 * @param item The item to add to the list
 * @param object The object to associate with the item
 */
static inline void ds_dlist_enq(ds_dlist_t *dlist, void *object)
{
    ds_dlist_item_t *item = DS_ITEM_OF(dlist, object);
    dlist->count++;
    item->next = 0;
    item->prev = dlist->last;
    dlist->last = item;
    if (!dlist->root)
        dlist->root = item;
    else // if (item->prev)
        item->prev->next = item;
}

/**
 * @brief Add an item at the front of the list
 *
 * @param dlist The list
 * @param item The item to add to the list
 * @param object The object to associate with the item
 */
static inline void ds_dlist_push(ds_dlist_t *dlist, void *object)
{
    ds_dlist_item_t *item = DS_ITEM_OF(dlist, object);
    dlist->count++;
    item->next = dlist->root;
    item->prev = 0;
    dlist->root = item;
    if (!dlist->last)
        dlist->last = item;
    else // if (item->next)
        item->next->prev = item;
}

/**
 * @brief Remove an object from the list
 *
 * @param dlist The list
 * @return The object
 */
static inline void *ds_dlist_remove(ds_dlist_t *dlist, void *object)
{
    ds_dlist_item_t *item = DS_ITEM_OF(dlist, object);
    if (dlist->root == item)
        dlist->root = item->next;
    else
        item->prev->next = item->next;
    if (dlist->last == item)
        dlist->last = item->prev;
    else
        item->next->prev = item->prev;
    dlist->count--;
    return object;
}

/**
 * @brief Remove an item from the list
 *
 * @param dlist The list
 * @return The object
 */
static inline void *ds_dlist_remove_item(ds_dlist_t *dlist, ds_dlist_item_t *item)
{
    return ds_dlist_remove(dlist, DS_OBJECT_OF(dlist, item));
}

#endif // __DS_DLIST_H__
