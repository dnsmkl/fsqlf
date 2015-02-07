#ifndef globals_h
#define globals_h


int currindent = 0;
int left_p  = 0;
int right_p = 0;


void inc_LEFTP();
void inc_RIGHTP();



typedef struct
{
    int left;
    int right;
} pair;


pair *make_pair_ptr(int l, int r)
{
    static pair tmp;
    tmp = (pair){l, r};
    return &tmp;
}




#include "../utils/stack/stack.h"


struct stack sub_openings;


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


#endif
