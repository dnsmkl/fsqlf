#ifndef global_variables_h
#define global_variables_h

#include <stdio.h>

int currindent=0;
int left_p  = 0;
int right_p = 0;
int subselect_level = 0;

int inc_LEFTP() { left_p++  ; }
int inc_RIGHTP(){ right_p++ ; }
int p_level()  { return left_p - right_p - subselect_level ; }


#define SUB_STACK_SIZE (100)
int sub_opening_leftps[SUB_STACK_SIZE];


void push_sub_stack(int left_p)
{   if(subselect_level < 0 || subselect_level >= SUB_STACK_SIZE) printf("\n --ERORR: push_sub_stack() - bounds\n");
    sub_opening_leftps[subselect_level] = left_p;
    subselect_level++;
}

int pop_sub_stack()
{   if(subselect_level <= 0 || subselect_level >= SUB_STACK_SIZE) printf("\n --ERORR: pop_sub_stack() - bounds\n");
    --subselect_level;
    return sub_opening_leftps[subselect_level];
}

int peek_sub_stack()
{   if(subselect_level < 0 || subselect_level >= SUB_STACK_SIZE) printf("\n --ERORR: peek_sub_stack() - bounds\n");
    if(subselect_level == 0) return 0;
    else return sub_opening_leftps[subselect_level-1];
}


int begin_SUB(){ push_sub_stack(left_p); currindent++; }
int end_SUB()  { pop_sub_stack(); currindent--; }




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
