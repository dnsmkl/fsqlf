#ifndef globals_h
#define globals_h


#include "../../utils/stack/stack.h"


// Global variables defined in globals.c file
extern int currindent;
extern int left_p;
extern int right_p;
extern struct stack state_stack;
extern struct stack sub_openings;


void inc_LEFTP();
void inc_RIGHTP();


typedef struct
{
    int left;
    int right;
} pair;


void begin_SUB();
void end_SUB();


char *dump_parenthesis_counts();


#endif
