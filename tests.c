#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "ds_heap.h"
#include "ds_lifo.h"
#include "ds_fifo.h"
#include "ds_dlist.h"
#include "ds_btree.h"
#include "ds_btree_ext.h"

#ifdef NDEBUG
    #define DO(X)
#else
    #define DO(X) X
#endif

#define ITEM_MAX 42
#define ERROR_MAX 150

typedef struct element_s element_t;
struct element_s
{
    ds_fifo_item_t fifo_item;
    ds_lifo_item_t lifo_item;
    ds_dlist_item_t dlist_item;
    ds_btree_item_t btree_item;
    int int1;
};

#define elementof(_ds, _item) ((element_t *)DS_OBJECT_OF(_ds, _item))
#define extelementof(_item) DS_EXT_OBJECT_OF(element_t, _item)

void lifo_print(ds_lifo_t *lifo)
{
    ds_lifo_item_t *item;
    for (item = lifo->root; item != 0; item = item->next)
        printf("%d ", elementof(lifo, item)->int1);
    printf("\n");
}

void fifo_print(ds_fifo_t *fifo)
{
    ds_fifo_item_t *item;
    for (item = fifo->root; item != 0; item = item->next)
        printf("%d ", elementof(fifo, item)->int1);
    printf("\n");
}

void dlist_print(ds_dlist_t *dlist)
{
    ds_dlist_item_t *item;
    for (item = dlist->root; item != 0; item = item->next)
        printf("%d ", elementof(dlist, item)->int1);
    printf("\n");
}

void btree_node_print(ds_btree_t *btree, ds_btree_item_t *node)
{
    if (node)
    {
        btree_node_print(btree, node->left);
        printf("%d ", elementof(btree, node)->int1);
        btree_node_print(btree, node->right);
    }
}

void btree_node_print_str(ds_btree_t *btree, ds_btree_ext_item_t *node)
{
    if (node)
    {
        btree_node_print_str(btree, node->left);
        printf("'%s' ", (char *)DS_EXT_OBJECT_OF(node));
        btree_node_print_str(btree, node->right);
    }
}

void btree_node_right_print(ds_btree_t *btree, ds_btree_item_t *node)
{
    if (node)
    {
        btree_node_right_print(btree, node->right);
        printf("%d ", elementof(btree, node)->int1);
        btree_node_right_print(btree, node->left);
    }
}

void btree_print(ds_btree_t *btree)
{
    btree_node_print(btree, btree->root);
    printf("\n");
}

void btree_print_str(ds_btree_t *btree)
{
    btree_node_print_str(btree, (ds_btree_ext_item_t *)btree->root);
    printf("\n");
}

void btree_right_print(ds_btree_t *btree)
{
    btree_node_right_print(btree, btree->root);
    printf("\n");
}

int btree_node_cmp(void *_left, void *_right)
{
    element_t *left = (element_t *)_left;
    element_t *right = (element_t *)_right;
    if (left->int1 == right->int1)
        return 0;
    if (left->int1 < right->int1)
        return -1;
    return 1;
}

static inline int btree_strcmp(void *_left, void *_right)
{
    char *left = (char *)_left;
    char *right = (char *)_right;
    return strcmp(left, right);
}

int main()
{
    DO(printf("# Data structure test\n"));

    DO(printf("# Create fifo, lifo, dlist and and btree\n"));
    ds_fifo_t fifo;
    ds_fifo_init(&fifo, offsetof(element_t, fifo_item));
    ds_lifo_t lifo;
    ds_lifo_init(&lifo, offsetof(element_t, lifo_item));
    ds_dlist_t dlist;
    ds_dlist_init(&dlist, offsetof(element_t, dlist_item));
    ds_btree_t btree;
    ds_btree_init(&btree, offsetof(element_t, btree_item), btree_node_cmp);

    DO(printf("# Create element_t heap with %d elements\n", ITEM_MAX));
    element_t *elements_store = calloc(ITEM_MAX, sizeof(element_t));
    ds_heap_t elements_heap;
    DS_HEAP_INIT(elements_heap, elements_store, ITEM_MAX, element_t);

    DO(printf("# Create a single element out of heap (4242)\n"));
    element_t the_4242_element = {.int1 = 4242};

    DO(printf("# Push the element (4242) in lifo\n"));
    ds_lifo_push(&lifo, &the_4242_element);
    DO(lifo_print(&lifo));

    DO(printf("# Allocate %d elements from heap and add them in fifo, lifo, dlist and btree\n", ITEM_MAX));
    DO(printf("# Duplicate btree element(s) not inserted:\n"));

    int i = 0;
    while (!DS_IS_EMPTY(&elements_heap.free_list))
    {
        element_t *element = ds_heap_alloc(&elements_heap);
        element->int1 = random() % 100;
        ds_fifo_enq(&fifo, element);
        ds_lifo_push(&lifo, element);
        if (i % 2 == 0)
            ds_dlist_push(&dlist, element);
        else
            ds_dlist_enq(&dlist, element);
        element_t *equal_object = ds_btree_insert(&btree, element);
        if (equal_object != element) // element is dupplicated
            DO(printf("%d ", element->int1));
        i++;
    }
    DO(printf("\n"));

    DO(printf("# fifo: the elements are in creation order\n"));
    DO(fifo_print(&fifo));
    DO(printf("# lifo: must have one more element than fifo (4242) and is in the reverse order of fifo\n"));
    DO(lifo_print(&lifo));
    DO(printf("# dlist: the first created element should be in the middle as we alternativaly pushed and queued elements\n"));
    DO(dlist_print(&dlist));
    DO(printf("# btree: elements are ordered given the cmp function\n"));
    DO(btree_print(&btree));
    DO(printf("# Adding another element in btree (4242)\n"));
    ds_btree_insert(&btree, &the_4242_element);
    DO(btree_print(&btree));
    DO(btree_right_print(&btree));

    DO(printf("\n# Remove all nodes from btree while dequeueing them from fifo\n# If remove succeeds on btree, also remove the removed object from dlist\n"));
    while (!DS_IS_EMPTY(&fifo))
    {
        element_t *element = ds_fifo_deq(&fifo);
        DO(printf("# Removing %d from btree, ", element->int1));
        element_t *removed = ds_btree_remove_object(&btree, element);
        if (removed)
        {
            ds_dlist_remove(&dlist, removed);
            DO(printf("also removed %d from dlist\n", removed->int1));
            DO(btree_print(&btree));
        }
        else
        {
            DO(printf("%d not found in btree\n", element->int1));
        }
    }
    DO(printf("# dlist: the dlist must contain duplicate(s)\n"));
    DO(dlist_print(&dlist));

    DO(printf("\n# Add elements in btree while poping them from lifo\n"));
    while (!DS_IS_EMPTY(&lifo))
    {
        element_t *element = ds_lifo_pop(&lifo);
        DO(printf("# Adding %d\n", element->int1));
        element_t *equal_object = ds_btree_insert(&btree, element);
        if (equal_object != element)
            DO(printf("# No node inserted: %d is duplicate\n", element->int1));
        else
            DO(btree_print(&btree));
    }

    char *errors[ERROR_MAX];
    ds_btree_ext_item_t error_items[ERROR_MAX];
    ds_btree_t error_tree;
    ds_btree_ext_init(&error_tree, (bs_btree_cmp_f)strcmp);

    for (int i = 0; i < ERROR_MAX; i++)
    {
        errors[i] = strerror(i);
        ds_btree_ext_insert(&error_tree, &error_items[i], errors[i]);
    }
    DO(printf("# Alpha ordered error string list (%zu items)\n", error_tree.count));
    DO(btree_print_str(&error_tree));
}
