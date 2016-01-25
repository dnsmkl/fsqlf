#ifndef tokque_h
#define tokque_h


#include <lib_fsqlf.h> // struct fsqlf_kw_conf, fsqlf_kw_get


struct FSQLF_state_change
{
    int state_change_action;
    int new_state;      // start conditions of flex lexer generator
};


// SCA - state change action
enum
{
    FSQLF_SCA_NONE,
    FSQLF_SCA_BEGIN,
    FSQLF_SCA_PUSH,
    FSQLF_SCA_POP
};


struct FSQLF_state_change FSQLF_tokque_putthrough(FILE *yyout, int *currindent,
    char *text, size_t len, const struct fsqlf_kw_conf *s, int cur_state);
void FSQLF_tokque_finish_out(FILE *yyout);


#endif
