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


#define STACK_T P(ITEM_T, _stack)
typedef struct
{
    // last item => items[length-1]
    // first item => items[0]
    // stack empty => length=0
    size_t length;
    ITEM_T items[STATE_STACK_SIZE];
} STACK_T;


void P(ITEM_T, _stack_init)(STACK_T* stk)
{
    stk->length = 0;
}


void P(ITEM_T, _stack_push)(STACK_T* stk, ITEM_T newitem)
{
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    stk->items[stk->length] = newitem;
    stk->length++;
}


ITEM_T P(ITEM_T, _stack_pop)(STACK_T* stk)
{
    stk->length--;
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    return stk->items[stk->length];
}


ITEM_T P(ITEM_T, _stack_peek)(const STACK_T* stk)
{
    assert(stk->length > 0 && stk->length < STATE_STACK_SIZE);
    return stk->items[stk->length-1];
}


int P(ITEM_T, _stack_empty)(const STACK_T* stk)
{
    return stk->length==0;
}


#undef ITEM_T

