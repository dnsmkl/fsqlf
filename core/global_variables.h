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
