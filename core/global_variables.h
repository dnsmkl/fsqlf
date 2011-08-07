#ifndef global_variables_h
#define global_variables_h

#include <stdio.h>

int currindent=0;
int left_p  = 0;
int right_p = 0;
int subselect_level = 0;
int white_space_cnt = 0;

int inc_LEFTP() { left_p++  ; }
int inc_RIGHTP(){ right_p++ ; }
int p_level()  { return left_p - right_p - subselect_level ; }
int begin_SUB(){ subselect_level++; currindent++; }
int end_SUB()  { subselect_level--; currindent--; }





#define STATE_STACK_SIZE (100)
int state_stack[STATE_STACK_SIZE];
int state_stack_index;


void init_stack()
{
    state_stack_index = 0;
}

void push_stack(int item)
{   if(state_stack_index < 0 || state_stack_index >= STATE_STACK_SIZE) printf("\n --ERORR: push_stack() - bounds\n");
    state_stack[state_stack_index] = item;
    state_stack_index++;
}

int pop_stack()
{   if(state_stack_index <= 0 || state_stack_index >= STATE_STACK_SIZE) printf("\n --ERORR: pop_stack() - bounds\n");
    --state_stack_index;
    return state_stack[state_stack_index];
}

int peek_stack()
{   if(state_stack_index <= 0 || state_stack_index >= STATE_STACK_SIZE) printf("\n --ERORR: peek_stack() - bounds\n");
    return state_stack[state_stack_index-1];
}


#endif
