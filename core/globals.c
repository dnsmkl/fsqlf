#include <stdio.h>  // sprintf
#include "globals.h"


int currindent = 0;
int left_p  = 0;
int right_p = 0;
struct stack state_stack;
struct stack sub_openings;


void inc_LEFTP()
{
    left_p++;
}


void inc_RIGHTP()
{
    right_p++;
}



pair *make_pair_ptr(int l, int r)
{
    static pair tmp;
    tmp = (pair){l, r};
    return &tmp;
}


void begin_SUB()
{
    stack_push(&sub_openings, make_pair_ptr(left_p, right_p));
    currindent++;
}


void end_SUB()
{
    stack_pop(&sub_openings);
    currindent--;
}


char *dump_paranthesis_counts()
{
    static char buffer[1000];
    sprintf(buffer, "'('=%d; ')'=%d; sub_openings.length=%d",
        left_p, right_p, sub_openings.length);
    return buffer;
}
