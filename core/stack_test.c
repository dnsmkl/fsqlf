#include <stdio.h>
#define ITEM_T int
#include "stack.h"


void int_stack_testing_suite()
{
    puts("stack_testing_suite:start");

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

    puts("stack_testing_suite:success\n");
}

void main()
{
    int_stack_testing_suite();
}
