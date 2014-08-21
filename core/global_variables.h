#ifndef global_variables_h
#define global_variables_h

#include <stdio.h>

int currindent=0;
int left_p  = 0;
int right_p = 0;
int subselect_level = 0;

void inc_LEFTP() { left_p++  ; }
void inc_RIGHTP(){ right_p++ ; }
int p_level()  { return left_p - right_p - subselect_level ; }


#define SUB_STACK_SIZE (100)
typedef struct para_st {
    int left;
    int right;
} para_st ;

para_st sub_openings[SUB_STACK_SIZE];


void push_sub_stack(int left_p, int right_p)
{   if(subselect_level < 0 || subselect_level >= SUB_STACK_SIZE) printf("\n --ERORR: push_sub_stack() - bounds\n");
    para_st tmp;
    tmp.left=left_p;
    tmp.right=right_p;
    sub_openings[subselect_level] = tmp;
    subselect_level++;
}

para_st pop_sub_stack()
{   if(subselect_level <= 0 || subselect_level >= SUB_STACK_SIZE) printf("\n --ERORR: pop_sub_stack() - bounds\n");
    --subselect_level;
    return sub_openings[subselect_level];
}

para_st peek_sub_stack()
{   if(subselect_level < 0 || subselect_level >= SUB_STACK_SIZE) printf("\n --ERORR: peek_sub_stack() - bounds\n");
    return sub_openings[subselect_level-1];
}


void begin_SUB(){ push_sub_stack(left_p,right_p); currindent++; }
void end_SUB()  { pop_sub_stack(); currindent--; }




#define ITEM_T int
#include "stack.h"
int_stack state_stack;


void init_stack()
{
    int_stack_init(&state_stack);
}

void push_stack(int item)
{
    int_stack_push(&state_stack, item);
}

int pop_stack()
{
    return int_stack_pop(&state_stack);
}

int peek_stack()
{
    return int_stack_peek(&state_stack);
}


#endif
