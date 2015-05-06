#ifndef tokque_h
#define tokque_h


#include "../kw/kw.h" // struct kw_conf, kw


struct state_change
{
    int change_needed;  // 1-yes; 0-no
    int new_state;      // start conditions of flex lexer generator
};


struct state_change tokque_putthrough(FILE *yyout,
    char *text, size_t len, const struct kw_conf *s);
void tokque_finish_out(FILE *yyout);


#endif
