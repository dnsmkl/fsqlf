// Generic stack (with lots of void*)


#include <assert.h>
#include <stdlib.h> // malloc, realloc
#include <string.h> // memcpy
#include "stack.h"



#define STATE_STACK_SIZE (100)


void stack_init(struct stack *stk, size_t isize)
{
    stk->length = 0;
    stk->isize = isize;
    stk->capacity = 100;
    stk->items = malloc(stk->isize * stk->capacity);
    assert(stk->items);
}


void stack_push(struct stack *stk, const void *newitem)
{
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    memcpy(stk->items + stk->length * stk->isize, newitem, stk->isize);
    stk->length++;
}


void *stack_pop(struct stack *stk)
{
    stk->length--;
    assert(stk->length >= 0 && stk->length < stk->capacity);
    return stk->items + stk->length * stk->isize;
}


void *stack_peek(const struct stack *stk)
{
    assert(stk->length > 0 && stk->length < STATE_STACK_SIZE);
    return stk->items + (stk->length-1) * stk->isize;
}


int stack_empty(const struct stack *stk)
{
    return stk->length == 0;
}

