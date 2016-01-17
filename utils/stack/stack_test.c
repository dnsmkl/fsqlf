#include <stdio.h>
#include <assert.h>
#include "stack.h"


void print_stack_struct(const struct FSQLF_stack *stk)
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

    struct FSQLF_stack t; // t=test
    FSQLF_stack_init(&t, sizeof(int));
    assert(FSQLF_stack_empty(&t));

    FSQLF_stack_push(&t, &(int){1});
    assert(!FSQLF_stack_empty(&t));
    assert(*(int*) FSQLF_stack_peek(&t) == 1);
    FSQLF_stack_push(&t, &(int){3});
    assert(*(int*) FSQLF_stack_peek(&t) == 3);
    FSQLF_stack_push(&t, &(int){9});
    assert(*(int*) FSQLF_stack_peek(&t) == 9);
    FSQLF_stack_push(&t, &(int){41});
    assert(*(int*) FSQLF_stack_peek(&t) == 41);

    assert(*(int*) FSQLF_stack_pop(&t) == 41);
    assert(*(int*) FSQLF_stack_peek(&t) == 9);
    assert(*(int*) FSQLF_stack_pop(&t) == 9);
    assert(*(int*) FSQLF_stack_peek(&t) == 3);
    assert(*(int*) FSQLF_stack_pop(&t) == 3);

    FSQLF_stack_push(&t, &(int){123});
    assert(*(int*) FSQLF_stack_peek(&t) == 123);

    assert(*(int*) FSQLF_stack_pop(&t) == 123);
    assert(*(int*) FSQLF_stack_peek(&t) == 1);
    assert(*(int*) FSQLF_stack_pop(&t) == 1);

    assert(FSQLF_stack_empty(&t));

    puts("stack_testing_suite:success\n");
}


void main()
{
    stack_testing_suite();
}
