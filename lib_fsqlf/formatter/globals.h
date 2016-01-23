#ifndef globals_h
#define globals_h


#include "../../utils/stack/stack.h"


// Global variables defined in globals.c file
extern struct FSQLF_stack FSQLF_state_stack;
extern struct FSQLF_stack FSQLF_sub_openings;


typedef struct
{
    int left;
    int right;
} pair;


#endif
