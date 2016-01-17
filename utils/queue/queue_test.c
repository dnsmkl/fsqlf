#define QUEUE_ITEM_T int
#include <assert.h>
#define FSQLF_QUEUE_INIT_CAPACITY (1)
#include "queue.h"
#include "queue.c"



void test_queue()
{
    QUEUE_ITEM_T item1 = 11;
    QUEUE_ITEM_T item2 = 22;
    QUEUE_ITEM_T item3 = 33;
    QUEUE_ITEM_T item4 = 44;

    QUEUE_ITEM_T tmp;

    struct FSQLF_queue tq;

    // At first
    FSQLF_queue_init(&tq, sizeof(int));
    // ..queue should be empty.
    assert(FSQLF_queue_empty(&tq));

    // After adding one item
    FSQLF_queue_push_back(&tq, &item1);
    // .. queue should become non-empty
    // .. and length should be 1.
    assert(!FSQLF_queue_empty(&tq));
    assert((tq.length) == 1);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 11);

    // Push_back some more items
    FSQLF_queue_push_back(&tq, &item2);
    FSQLF_queue_push_back(&tq, &item3);
    FSQLF_queue_push_back(&tq, &item4);
    // .. and do the checks again.
    assert(!FSQLF_queue_empty(&tq));
    assert(tq.length==4);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 11);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 22);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 2) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 3) == 44);

    // Now drop_head and push_back couple of times repeatedly.
    // (to check if wrapping works correctly)
    FSQLF_queue_drop_head(&tq);
    assert(tq.length==3);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 22);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 2) == 44);

    FSQLF_queue_push_back(&tq, &item3);
    assert(tq.length==4);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 22);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 2) == 44);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 3) == 33);

    FSQLF_queue_drop_head(&tq);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 44);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 2) == 33);

    FSQLF_queue_push_back(&tq, &item2);
    assert(tq.length==4);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 44);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 2) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 3) == 22);

    FSQLF_queue_drop_head(&tq);
    assert(tq.length==3);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 44);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 2) == 22);

    FSQLF_queue_push_back(&tq, &item1);
    assert(tq.length==4);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 44);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 2) == 22);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 3) == 11);

    FSQLF_queue_push_back(&tq, &item2);
    FSQLF_queue_push_back(&tq, &item1);
    FSQLF_queue_push_back(&tq, &item3);
    FSQLF_queue_push_back(&tq, &item4);

    assert(tq.length==8);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 44);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 2) == 22);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 3) == 11);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 4) == 22);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 5) == 11);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 6) == 33);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 7) == 44);

    // Now dropp till it's empty.
    FSQLF_queue_drop_head(&tq); assert(tq.length == 7);
    FSQLF_queue_drop_head(&tq); assert(tq.length == 6);
    FSQLF_queue_drop_head(&tq); assert(tq.length == 5);
    FSQLF_queue_drop_head(&tq); assert(tq.length == 4);
    FSQLF_queue_drop_head(&tq); assert(tq.length == 3);
    FSQLF_queue_drop_head(&tq); assert(tq.length == 2);
    FSQLF_queue_drop_head(&tq); assert(tq.length == 1);
    FSQLF_queue_drop_head(&tq); assert(tq.length == 0);
    assert(FSQLF_queue_empty(&tq));
}


void test_queue_internals()
{
    struct FSQLF_queue tq;
    FSQLF_queue_init(&tq, sizeof(int));

    FSQLF_queue_push_back(&tq, &(int){0});
    FSQLF_queue_push_back(&tq, &(int){1});

    assert(tq.start == 0);
    assert(tq.length == 2);
    assert(tq.capacity == 2);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 0);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 1);

    FSQLF_queue_drop_head(&tq);
    FSQLF_queue_push_back(&tq, &(int){2});

    assert(tq.start == 1);
    assert(tq.length == 2);
    assert(tq.capacity == 2);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 1);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 2);

    FSQLF_queue_drop_head(&tq);
    FSQLF_queue_push_back(&tq, &(int){3});

    assert(tq.start == 0);
    assert(tq.length == 2);
    assert(tq.capacity == 2);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 2);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 3);

    FSQLF_queue_drop_head(&tq);
    FSQLF_queue_push_back(&tq, &(int){4});

    assert(tq.start == 1);
    assert(tq.length == 2);
    assert(tq.capacity == 2);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 0) == 3);
    assert(*(int*)FSQLF_queue_peek_n(&tq, 1) == 4);
}


void test_queue_array_pos()
{
    size_t i;
    for (i = 0; i < 10; i++) assert(queue_array_pos(i, 0, 10) == i);

    // Lets imagine internal array of capacity 5
    // and queue that starts at element 2 in internal array:
    //     0 1 2 3 4 <- internal array positions
    //     3 4 0 1 2 <- queue elements
    assert(queue_array_pos(0, 2, 5) == 2);
    assert(queue_array_pos(1, 2, 5) == 3);
    assert(queue_array_pos(2, 2, 5) == 4);
    assert(queue_array_pos(3, 2, 5) == 0);
    assert(queue_array_pos(4, 2, 5) == 1);
}


int main()
{
    test_queue_array_pos();
    test_queue_internals();
    test_queue();
    return 0;
}

