// Queue implementation.
///
// Macros for configuration:
//  QUEUE_INIT_CAPACITY - initial capacity (optional; defaults to 100)
///
// Defined names for queue usage:
// (other names should not be used)
//  struct queue - queue struct
//  struct queue.length - number of items in the queue (size_t).
// Queue manipulation:
//  void queue_init(struct queue*) - initialize queue.
//  void queue_clear(struct queue*) - free resources, make queue unusable.
//  void queue_push_back(struct queue*, void*) - Add item onto the back.
//  void queue_drop_head(struct queue*) - Remove item from head.
//  void *queue_peek_n(struct queue*, size_t) - Return certain element.
//  int queue_empty(struct queue*) - Check if queue is empty.


#ifndef token_queue_h
#define token_queue_h


#include <stdlib.h> // malloc, realloc
#include <string.h> // memcpy
#include <assert.h> // assert


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


// Helper function for converting queue position to internal array position.
size_t queue_array_pos(size_t que_n, size_t que_start, size_t arr_capacity);
// Helper function for increasing capacity of internal array.
void queue_increase_capacity(struct queue * q);


void queue_init(struct queue * const q, size_t isize)
{
    q->length = 0;
    q->start = 0;
    q->capacity = QUEUE_INIT_CAPACITY;
    q->isize = isize;
    q->items = (char*) malloc(q->isize * q->capacity);
    assert(q->items != NULL);
}


void queue_clear(struct queue * const q)
{
    q->length = 0;
    q->start = 0;
    q->capacity = 0;
    free(q->items);
}


void queue_push_back(struct queue * const q, const void * const item)
{
    if (q->length == q->capacity) {
        queue_increase_capacity(q);
    }
    assert(q->length < q->capacity);
    size_t arr_pos = queue_array_pos(q->length, q->start, q->capacity);
    memcpy(q->items + arr_pos * q->isize, item, q->isize);
    q->length++;
}


void queue_drop_head(struct queue * const q)
{
    assert(q->length > 0);
    q->start++;
    if (q->start == q->capacity) q->start = 0; // Wrap past the end.
    q->length--;
}


void *queue_peek_n(const struct queue * const q, const size_t n)
{
    assert(n < q->length);
    assert(q->length <= q->capacity);
    size_t arr_pos = queue_array_pos(n, q->start, q->capacity);
    return q->items + arr_pos * q->isize;
}


int queue_empty(const struct queue * const q)
{
    return q->length == 0;
}


// Helper function for converting queue position to internal array position.
// Queue is implemented in array, where queue can start not at zero
// and elements can wrap past the end.
// (without such wrapping capabilities
// it would be needed to constantly re-order elements)
///
// Lets imagine internal array of capacity 5
// and queue that starts at element 2 in internal array:
//     0 1 2 3 4 <- internal array positions
//     3 4 0 1 2 <- queue elements
size_t queue_array_pos(const size_t que_n,
    const size_t que_start,
    const size_t arr_capacity)
{
    if (que_n < 0) assert(0);
    if (que_n >= arr_capacity) assert(0);
    if (arr_capacity == 0) assert(0);

    size_t size_till_end = arr_capacity-que_start;
    size_t r = que_n < size_till_end ?
        que_n + que_start :     // If fits into remaining array.
        que_n - size_till_end;  // If element wrapps past the end.

    assert(r >= 0);
    assert(r < arr_capacity);

    return r;
}


// Helper function for increasing capacity of internal array.
void queue_increase_capacity(struct queue * const q)
{
    size_t old_cap = q->capacity;
    q->capacity *= 2;
    q->items = (char*) realloc(q->items, q->isize * q->capacity);
    assert(q->items != NULL);
    // Copy elements that had to wrap past end,
    // to new space available at the end.
    size_t i, old_pos, new_pos;
    for (i = 0; i < (q->length); i++) {
        old_pos = queue_array_pos(i, q->start, old_cap);
        new_pos = queue_array_pos(i, q->start, q->capacity);
        memcpy(q->items + new_pos * q->isize,
                q->items + old_pos * q->isize, q->isize);
    }
}


#endif
