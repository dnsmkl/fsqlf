#ifndef STACK_H
#define STACK_H

#include <assert.h>

#define STATE_STACK_SIZE (100)


#define PASTE2(a,b) a ## b
#define P(a,b) PASTE2(a,b)


#define ITEM_T int
typedef struct
{
    // last item => items[length-1]
    // first item => items[0]
    // stack empty => length=0
    size_t length;
    ITEM_T items[STATE_STACK_SIZE];
} P(ITEM_T, _stack);


void P(ITEM_T, _stack_init)(P(ITEM_T, _stack)* stk)
{
    stk->length = 0;
}


void P(ITEM_T, _stack_push)(P(ITEM_T, _stack)* stk, ITEM_T newitem)
{
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    stk->items[stk->length] = newitem;
    stk->length++;
}


ITEM_T P(ITEM_T, _stack_pop)(P(ITEM_T, _stack)* stk)
{
    stk->length--;
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    return stk->items[stk->length];
}


ITEM_T P(ITEM_T, _stack_peek)(const P(ITEM_T, _stack)* stk)
{
    assert(stk->length > 0 && stk->length < STATE_STACK_SIZE);
    return stk->items[stk->length-1];
}


int P(ITEM_T, _stack_empty)(const P(ITEM_T, _stack)* stk)
{
    return stk->length==0;
}


void P(ITEM_T, _stack_dump)(P(ITEM_T, _stack) stk)
{
    printf("%d:%d %d %d %d\n", stk.length, stk.items[0], stk.items[1], stk.items[2], stk.items[3]);
    fflush(stdout);
}


#endif

