#ifndef STACK_H
#define STACK_H


struct stack
{
    // last item => items[length-1]
    // first item => items[0]
    // stack empty => length=0
    size_t length;
    size_t isize;
    size_t capacity;
    char *items;
};


void stack_init(struct stack *stk, size_t isize);
void stack_push(struct stack *stk, const void *newitem);
void *stack_pop(struct stack *stk);
void *stack_peek(const struct stack *stk);
int stack_empty(const struct stack *stk);


#endif
// gcc -c stack.c -o stack.o  && gcc -c stack_test_new.c -o stack_test_new.o && gcc stack_test_new.o stack.o && ./a.out
