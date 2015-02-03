#ifndef global_variables_h
#define global_variables_h


int currindent = 0;
int left_p  = 0;
int right_p = 0;


void inc_LEFTP()
{
    left_p++;
}


void inc_RIGHTP()
{
    right_p++;
}


typedef struct
{
    int left;
    int right;
} pair;


pair make_pair(int l, int r)
{
    pair tmp = {l, r};
    return tmp;
}




#define ITEM_T pair
#include "../utils/stack/stack.h"
//  void <t>_stack_init(<t>_stack* stk)
//  void <t>_stack_push(<t>_stack* stk, <t> newitem)
//  <t> <t>_stack_pop(<t>_stack* stk)
//  <t> <t>_stack_peek(const <t>_stack* stk)
//  int <t>_stack_empty(const <t>_stack* stk)


struct pair_stack sub_openings;


void begin_SUB()
{
    pair_stack_push(&sub_openings, make_pair(left_p, right_p));
    currindent++;
}


void end_SUB()
{
    pair_stack_pop(&sub_openings);
    currindent--;
}



char *dump_paranthesis_counts()
{
    char buffer[1000];
    sprintf(buffer, "'('=%d; ')'=%d; sub_openings.length=%d",
        left_p, right_p, sub_openings.length);
    return buffer;
}


#endif
