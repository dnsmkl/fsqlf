#ifndef STACK_H
#define STACK_H

#include <assert.h>

#define STATE_STACK_SIZE (100)

typedef int stackitem;

typedef struct
{
    // last item => items[length-1]
    // first item => items[0]
    // stack empty => length=0
    size_t length;
    stackitem items[STATE_STACK_SIZE];
} stack;


void stack_init(stack* stk)
{
    stk->length = 0;
}


void stack_push(stack* stk, stackitem newitem)
{
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    stk->items[stk->length] = newitem;
    stk->length++;
}


stackitem stack_pop(stack* stk)
{
    stk->length--;
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    return stk->items[stk->length];
}


stackitem stack_peek(const stack* stk)
{
    assert(stk->length > 0 && stk->length < STATE_STACK_SIZE);
    return stk->items[stk->length-1];
}


int stack_empty(const stack* stk)
{
    return stk->length==0;
}


void stack_dump(stack stk)
{
    printf("%d:%d %d %d %d\n", stk.length, stk.items[0], stk.items[1], stk.items[2], stk.items[3]);
    fflush(stdout);
}


//#include <stdio.h>
void stack_testing_suite()
{
    //puts("stack_testing_suite:start\n");
    stack t; // t=test
    stack_init(&t);
    assert(stack_empty(&t));

    stack_push(&t,1);
    assert(!stack_empty(&t));
    assert(stack_peek(&t)==1);
    stack_push(&t,3);
    assert(stack_peek(&t)==3);
    stack_push(&t,9);
    assert(stack_peek(&t)==9);
    stack_push(&t,41);
    assert(stack_peek(&t)==41);

    assert(stack_pop(&t)==41);
    assert(stack_peek(&t)==9);
    assert(stack_pop(&t)==9);
    assert(stack_peek(&t)==3);
    assert(stack_pop(&t)==3);

    stack_push(&t,123);
    assert(stack_peek(&t)==123);

    assert(stack_pop(&t)==123);
    assert(stack_peek(&t)==1);
    assert(stack_pop(&t)==1);

    assert(stack_empty(&t));
    //puts("stack_testing_suite:success\n");
}


#endif

