#ifndef tokque_h
#define tokque_h


#include <lib_fsqlf.h> // struct kw_conf, kw


struct state_change
{
    int state_change_action;
    int new_state;      // start conditions of flex lexer generator
};


// SCA - state change action
enum
{
    SCA_NONE,
    SCA_BEGIN,
    SCA_PUSH,
    SCA_POP
};


struct state_change tokque_putthrough(FILE *yyout, int *currindent,
    char *text, size_t len, const struct kw_conf *s, int cur_state);
void tokque_finish_out(FILE *yyout);


#endif
