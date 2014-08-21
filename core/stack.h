#ifndef STACK_H
#define STACK_H


// Generic stack implementation.
// ITEM_T macro has to be defined before inclusion.
//
// List of functions defined:
//  void <t>_stack_init(<t>_stack* stk)
//  void <t>_stack_push(<t>_stack* stk, <t> newitem)
//  <t> <t>_stack_pop(<t>_stack* stk)
//  <t> <t>_stack_peek(const <t>_stack* stk)
//  int <t>_stack_empty(const <t>_stack* stk)


#include <assert.h>

#define STATE_STACK_SIZE (100)


#define PASTE2(a,b) a ## b
#define P(a,b) PASTE2(a,b)


#ifndef ITEM_T
#error `ITEM_T` must be defined before the: #include "stack.h"
#endif


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


#endif

