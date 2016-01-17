// Generic stack (with lots of void*)


#include <assert.h>
#include <stdlib.h> // malloc, realloc
#include <string.h> // memcpy
#include "stack.h"



#define STATE_STACK_SIZE (100)


void FSQLF_stack_init(struct FSQLF_stack *stk, size_t isize)
{
    stk->length = 0;
    stk->isize = isize;
    stk->capacity = 100;
    stk->items = malloc(stk->isize * stk->capacity);
    assert(stk->items);
}


void FSQLF_stack_push(struct FSQLF_stack *stk, const void *newitem)
{
    assert(stk->length >= 0 && stk->length < STATE_STACK_SIZE);
    memcpy(stk->items + stk->length * stk->isize, newitem, stk->isize);
    stk->length++;
}


void *FSQLF_stack_pop(struct FSQLF_stack *stk)
{
    stk->length--;
    assert(stk->length >= 0 && stk->length < stk->capacity);
    return stk->items + stk->length * stk->isize;
}


void *FSQLF_stack_peek(const struct FSQLF_stack *stk)
{
    assert(stk->length > 0 && stk->length < STATE_STACK_SIZE);
    return stk->items + (stk->length-1) * stk->isize;
}


int FSQLF_stack_empty(const struct FSQLF_stack *stk)
{
    return stk->length == 0;
}

