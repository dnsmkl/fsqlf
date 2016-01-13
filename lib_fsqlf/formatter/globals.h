#ifndef globals_h
#define globals_h


#include "../../utils/stack/stack.h"


// Global variables defined in globals.c file
extern struct stack state_stack;
extern struct stack sub_openings;


typedef struct
{
    int left;
    int right;
} pair;


#endif
