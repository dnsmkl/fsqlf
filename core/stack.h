#ifndef STACK_H
#define STACK_H

#include <assert.h>

#define STATE_STACK_SIZE (100)


typedef struct
{
    // last item => items[length-1]
    // first item => items[0]
    // stack empty => length=0
    size_t length;
    int items[STATE_STACK_SIZE];
} int_stack;


void int_stack_init(int_stack* stk)
{
    stk->length = 0;
}


void int_stack_push(int_stack* stk, int newitem)
{
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    stk->items[stk->length] = newitem;
    stk->length++;
}


int int_stack_pop(int_stack* stk)
{
    stk->length--;
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    return stk->items[stk->length];
}


int int_stack_peek(const int_stack* stk)
{
    assert(stk->length > 0 && stk->length < STATE_STACK_SIZE);
    return stk->items[stk->length-1];
}


int int_stack_empty(const int_stack* stk)
{
    return stk->length==0;
}


void int_stack_dump(int_stack stk)
{
    printf("%d:%d %d %d %d\n", stk.length, stk.items[0], stk.items[1], stk.items[2], stk.items[3]);
    fflush(stdout);
}


//#include <stdio.h>
void int_stack_testing_suite()
{
    //puts("stack_testing_suite:start\n");
    int_stack t; // t=test
    int_stack_init(&t);
    assert(int_stack_empty(&t));

    int_stack_push(&t,1);
    assert(!int_stack_empty(&t));
    assert(int_stack_peek(&t)==1);
    int_stack_push(&t,3);
    assert(int_stack_peek(&t)==3);
    int_stack_push(&t,9);
    assert(int_stack_peek(&t)==9);
    int_stack_push(&t,41);
    assert(int_stack_peek(&t)==41);

    assert(int_stack_pop(&t)==41);
    assert(int_stack_peek(&t)==9);
    assert(int_stack_pop(&t)==9);
    assert(int_stack_peek(&t)==3);
    assert(int_stack_pop(&t)==3);

    int_stack_push(&t,123);
    assert(int_stack_peek(&t)==123);

    assert(int_stack_pop(&t)==123);
    assert(int_stack_peek(&t)==1);
    assert(int_stack_pop(&t)==1);

    assert(int_stack_empty(&t));
    //puts("stack_testing_suite:success\n");
}


#endif

