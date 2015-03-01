// Queue implementation.
///
// Macros for configuration:
//  QUEUE_INIT_CAPACITY - initial capacity (optional; defaults to 100)
///
// Defined names for queue usage:
// (other names should not be used)
//  struct queue - queue struct
//  struct queue.length - number of items in the queue (size_t).
//  void queue_init(struct queue*) - initialize queue.
//  void queue_clear(struct queue*) - free resources, make queue unusable.
//  void queue_push_back(struct queue*, void*) - Add item onto the back.
//  void queue_drop_head(struct queue*) - Remove item from head.
//  void *queue_peek_n(struct queue*, size_t) - Return certain element.
//  int queue_empty(struct queue*) - Check if queue is empty.


#ifndef token_queue_h
#define token_queue_h


#include <string.h>


#ifndef QUEUE_INIT_CAPACITY
#define QUEUE_INIT_CAPACITY (100)
#endif


struct queue
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


void queue_init(struct queue * const q, size_t isize);
void queue_clear(struct queue * const q);
void queue_push_back(struct queue * const q, const void * const item);
void queue_drop_head(struct queue * const q);
void *queue_peek_n(const struct queue * const q, const size_t n);
int queue_empty(const struct queue * const q);


// Helper function for converting queue position to internal array position.
size_t queue_array_pos(size_t que_n, size_t que_start, size_t arr_capacity);
// Helper function for increasing capacity of internal array.
void queue_increase_capacity(struct queue * q);


#endif
