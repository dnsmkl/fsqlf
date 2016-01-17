#ifndef STACK_H
#define STACK_H


struct FSQLF_stack
{
    // last item => items[length-1]
    // first item => items[0]
    // stack empty => length=0
    size_t length;
    size_t isize;
    size_t capacity;
    char *items;
};


void FSQLF_stack_init(struct FSQLF_stack *stk, size_t isize);
void FSQLF_stack_push(struct FSQLF_stack *stk, const void *newitem);
void *FSQLF_stack_pop(struct FSQLF_stack *stk);
void *FSQLF_stack_peek(const struct FSQLF_stack *stk);
int FSQLF_stack_empty(const struct FSQLF_stack *stk);


#endif
// gcc -c stack.c -o stack.o  && gcc -c stack_test_new.c -o stack_test_new.o && gcc stack_test_new.o stack.o && ./a.out
