// Queue implementation.
///
// Macros for configuration:
//  FSQLF_QUEUE_INIT_CAPACITY - initial capacity (optional; defaults to 16)
///
// Defined names for queue usage:
// (other names should not be used)
//  struct FSQLF_queue - queue struct
//  struct FSQLF_queue.length - number of items in the queue (size_t).
//  void FSQLF_queue_init(struct FSQLF_queue*) - initialize queue.
//  void FSQLF_queue_clear(struct FSQLF_queue*) - free resources, make queue unusable.
//  void *FSQLF_queue_alloc_back(struct FSQLF_queue * const q) - Allocate item onto the back.
//  void FSQLF_queue_push_back(struct FSQLF_queue*, void*) - Add item onto the back.
//  void FSQLF_queue_drop_head(struct FSQLF_queue*) - Remove item from head.
//  void *FSQLF_queue_peek_n(struct FSQLF_queue*, size_t) - Return certain element.
//  int FSQLF_queue_empty(struct FSQLF_queue*) - Check if queue is empty.


#ifndef QUEUE_H
#define QUEUE_H


#include <string.h>


#ifndef FSQLF_QUEUE_INIT_CAPACITY
#define FSQLF_QUEUE_INIT_CAPACITY (16)
#endif


struct FSQLF_queue
{
    // Internal array for item storage.
    // (see queue_array_pos() for info about positions used for queue items)
    char *items;

    // Position of first queue item in internal array.
    size_t start;

    // Size of the queue - number of elements currently in the queue.
    size_t length;

    // Size of internal array - max number of elements possible without realloc.
    size_t capacity;

    // Size of individual item.
    size_t isize;
};


void FSQLF_queue_init(struct FSQLF_queue * const q, size_t isize);
void FSQLF_queue_clear(struct FSQLF_queue * const q);
void *FSQLF_queue_alloc_back(struct FSQLF_queue * const q);
void FSQLF_queue_push_back(struct FSQLF_queue * const q, const void * const item);
void FSQLF_queue_drop_head(struct FSQLF_queue * const q);
void *FSQLF_queue_peek_n(const struct FSQLF_queue * const q, const size_t n);
int FSQLF_queue_empty(const struct FSQLF_queue * const q);


#endif
