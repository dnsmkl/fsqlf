#include <stdio.h>
#include <assert.h>
#include "stack.h"


void print_stack_struct(const struct stack *stk)
{
    int i;
    printf("Dump: %d %d", stk->length, stk->isize);
    for (i = 0; i < stk->length * stk->isize; ++i) {
        if (i % stk->isize == 0) printf("\n");
        printf("%d ", ((char*)stk->items)[i]);
    }
    puts("\n");
    fflush(stdout);
}


void stack_testing_suite()
{
    puts("stack_testing_suite:start");

    struct stack t; // t=test
    stack_init(&t, sizeof(int));
    assert(stack_empty(&t));

    stack_push(&t, &(int){1});
    assert(!stack_empty(&t));
    assert(*(int*) stack_peek(&t) == 1);
    stack_push(&t, &(int){3});
    assert(*(int*) stack_peek(&t) == 3);
    stack_push(&t, &(int){9});
    assert(*(int*) stack_peek(&t) == 9);
    stack_push(&t, &(int){41});
    assert(*(int*) stack_peek(&t) == 41);

    assert(*(int*) stack_pop(&t) == 41);
    assert(*(int*) stack_peek(&t) == 9);
    assert(*(int*) stack_pop(&t) == 9);
    assert(*(int*) stack_peek(&t) == 3);
    assert(*(int*) stack_pop(&t) == 3);

    stack_push(&t, &(int){123});
    assert(*(int*) stack_peek(&t) == 123);

    assert(*(int*) stack_pop(&t) == 123);
    assert(*(int*) stack_peek(&t) == 1);
    assert(*(int*) stack_pop(&t) == 1);

    assert(stack_empty(&t));

    puts("stack_testing_suite:success\n");
}


void main()
{
    stack_testing_suite();
}
