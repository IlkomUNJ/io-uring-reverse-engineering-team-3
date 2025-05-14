#ifndef INTERNAL_IO_SLIST_H
#define INTERNAL_IO_SLIST_H

#include <linux/io_uring_types.h>

#define __wq_list_for_each(pos, head)				\
	for (pos = (head)->first; pos; pos = (pos)->next)

#define wq_list_for_each(pos, prv, head)			\
	for (pos = (head)->first, prv = NULL; pos; prv = pos, pos = (pos)->next)

#define wq_list_for_each_resume(pos, prv)			\
	for (; pos; prv = pos, pos = (pos)->next)

#define wq_list_empty(list)	(READ_ONCE((list)->first) == NULL)

#define INIT_WQ_LIST(list)	do {				\
	(list)->first = NULL;					\
} while (0)

/**
 * Add a node after a specified position in the work queue list.
 * Updates the list's last pointer if the new node is added at the end.
 */
static inline void wq_list_add_after(struct io_wq_work_node *node,
                     struct io_wq_work_node *pos,
                     struct io_wq_work_list *list)
{
    struct io_wq_work_node *next = pos->next;

    pos->next = node;
    node->next = next;
    if (!next)
        list->last = node;
}

/**
 * Add a node to the tail of the work queue list.
 * Updates the list's first and last pointers as necessary.
 */
static inline void wq_list_add_tail(struct io_wq_work_node *node,
                    struct io_wq_work_list *list)
{
    node->next = NULL;
    if (!list->first) {
        list->last = node;
        WRITE_ONCE(list->first, node);
    } else {
        list->last->next = node;
        list->last = node;
    }
}

/**
 * Add a node to the head of the work queue list.
 * Updates the list's first and last pointers as necessary.
 */
static inline void wq_list_add_head(struct io_wq_work_node *node,
                    struct io_wq_work_list *list)
{
    node->next = list->first;
    if (!node->next)
        list->last = node;
    WRITE_ONCE(list->first, node);
}

/**
 * Remove a range of nodes from the work queue list.
 * Updates the list's first and last pointers as necessary.
 */
static inline void wq_list_cut(struct io_wq_work_list *list,
                   struct io_wq_work_node *last,
                   struct io_wq_work_node *prev)
{
    /* first in the list, if prev==NULL */
    if (!prev)
        WRITE_ONCE(list->first, last->next);
    else
        prev->next = last->next;

    if (last == list->last)
        list->last = prev;
    last->next = NULL;
}

/**
 * Splice the contents of one work queue list into another at a specified position.
 * Resets the source list after splicing.
 */
static inline void __wq_list_splice(struct io_wq_work_list *list,
                    struct io_wq_work_node *to)
{
    list->last->next = to->next;
    to->next = list->first;
    INIT_WQ_LIST(list);
}

/**
 * Splice the contents of one work queue list into another at a specified position.
 * Returns true if the source list was not empty and splicing occurred.
 */
static inline bool wq_list_splice(struct io_wq_work_list *list,
                  struct io_wq_work_node *to)
{
    if (!wq_list_empty(list)) {
        __wq_list_splice(list, to);
        return true;
    }
    return false;
}

/**
 * Add a node to the head of a stack.
 * Updates the stack's next pointer.
 */
static inline void wq_stack_add_head(struct io_wq_work_node *node,
                     struct io_wq_work_node *stack)
{
    node->next = stack->next;
    stack->next = node;
}

/**
 * Delete a node from the work queue list.
 * Uses wq_list_cut to handle the removal.
 */
static inline void wq_list_del(struct io_wq_work_list *list,
                   struct io_wq_work_node *node,
                   struct io_wq_work_node *prev)
{
    wq_list_cut(list, node, prev);
}

/**
 * Extract the top node from a stack.
 * Updates the stack's next pointer to remove the extracted node.
 */
static inline
struct io_wq_work_node *wq_stack_extract(struct io_wq_work_node *stack)
{
    struct io_wq_work_node *node = stack->next;

    stack->next = node->next;
    return node;
}

/**
 * Retrieve the next work item from the work queue.
 * Returns NULL if there is no next work item.
 */
static inline struct io_wq_work *wq_next_work(struct io_wq_work *work)
{
    if (!work->list.next)
        return NULL;

    return container_of(work->list.next, struct io_wq_work, list);
}

#endif // INTERNAL_IO_SLIST_H
