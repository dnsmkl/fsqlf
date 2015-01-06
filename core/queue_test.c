#define QUEUE_INIT_CAPACITY 1
#define QUEUE_ITEM_T int
#include "queue.h"



void test_queue()
{
    QUEUE_ITEM_T item1 = 11;
    QUEUE_ITEM_T item2 = 22;
    QUEUE_ITEM_T item3 = 33;
    QUEUE_ITEM_T item4 = 44;

    QUEUE_ITEM_T tmp;

    queue_t tq;

    // At first
    queue_init(&tq);
    // ..queue should be empty.
    assert(queue_empty(&tq));

    // After adding one item
    queue_push_back(&tq, item1);
    // .. queue should become non-empty
    // .. and length should be 1.
    assert(!queue_empty(&tq));
    assert((tq.length)==1);
    assert(queue_peek_n(&tq, 0) == 11);

    // Push_back some more items
    queue_push_back(&tq, item2);
    queue_push_back(&tq, item3);
    queue_push_back(&tq, item4);
    // .. and do the checks again.
    assert(!queue_empty(&tq));
    assert(tq.length==4);
    assert(queue_peek_n(&tq, 0) == 11);
    assert(queue_peek_n(&tq, 1) == 22);
    assert(queue_peek_n(&tq, 2) == 33);
    assert(queue_peek_n(&tq, 3) == 44);

    // Now drop_head and push_back couple of times repeatedly.
    // (to check if wrapping works correctly)
    queue_drop_head(&tq);
    assert(tq.length==3);
    assert(queue_peek_n(&tq, 0) == 22);
    assert(queue_peek_n(&tq, 1) == 33);
    assert(queue_peek_n(&tq, 2) == 44);

    queue_push_back(&tq, item3);
    assert(tq.length==4);
    assert(queue_peek_n(&tq, 0) == 22);
    assert(queue_peek_n(&tq, 1) == 33);
    assert(queue_peek_n(&tq, 2) == 44);
    assert(queue_peek_n(&tq, 3) == 33);

    queue_drop_head(&tq);
    assert(queue_peek_n(&tq, 0) == 33);
    assert(queue_peek_n(&tq, 1) == 44);
    assert(queue_peek_n(&tq, 2) == 33);

    queue_push_back(&tq, item2);
    assert(tq.length==4);
    assert(queue_peek_n(&tq, 0) == 33);
    assert(queue_peek_n(&tq, 1) == 44);
    assert(queue_peek_n(&tq, 2) == 33);
    assert(queue_peek_n(&tq, 3) == 22);

    queue_drop_head(&tq);
    assert(tq.length==3);
    assert(queue_peek_n(&tq, 0) == 44);
    assert(queue_peek_n(&tq, 1) == 33);
    assert(queue_peek_n(&tq, 2) == 22);

    queue_push_back(&tq, item1);
    assert(tq.length==4);
    assert(queue_peek_n(&tq, 0) == 44);
    assert(queue_peek_n(&tq, 1) == 33);
    assert(queue_peek_n(&tq, 2) == 22);
    assert(queue_peek_n(&tq, 3) == 11);

    queue_push_back(&tq, item2);
    queue_push_back(&tq, item1);
    queue_push_back(&tq, item3);
    queue_push_back(&tq, item4);

    assert(tq.length==8);
    assert(queue_peek_n(&tq, 0) == 44);
    assert(queue_peek_n(&tq, 1) == 33);
    assert(queue_peek_n(&tq, 2) == 22);
    assert(queue_peek_n(&tq, 3) == 11);
    assert(queue_peek_n(&tq, 4) == 22);
    assert(queue_peek_n(&tq, 5) == 11);
    assert(queue_peek_n(&tq, 6) == 33);
    assert(queue_peek_n(&tq, 7) == 44);

    // Now dropp till it's empty.
    queue_drop_head(&tq); assert(tq.length==7);
    queue_drop_head(&tq); assert(tq.length==6);
    queue_drop_head(&tq); assert(tq.length==5);
    queue_drop_head(&tq); assert(tq.length==4);
    queue_drop_head(&tq); assert(tq.length==3);
    queue_drop_head(&tq); assert(tq.length==2);
    queue_drop_head(&tq); assert(tq.length==1);
    queue_drop_head(&tq); assert(tq.length==0);
    assert(queue_empty(&tq));
}


void test_qpos_to_apos()
{
    size_t i;
    for(i=0; i<10; i++) assert(qpos_to_apos(i, 0, 10)==i);

    // Lets imagine internal array of capacity 5
    // and queue that starts at element 2 in internal array:
    //     0 1 2 3 4 <- internal array positions
    //     3 4 0 1 2 <- queue elements
    assert(qpos_to_apos(0, 2, 5)==2);
    assert(qpos_to_apos(1, 2, 5)==3);
    assert(qpos_to_apos(2, 2, 5)==4);
    assert(qpos_to_apos(3, 2, 5)==0);
    assert(qpos_to_apos(4, 2, 5)==1);
}


int main()
{
    test_queue();
    test_qpos_to_apos();
    return 0;
}

