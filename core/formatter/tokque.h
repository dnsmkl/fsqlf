#ifndef tokque_h
#define tokque_h


#include "../kw/kw.h" // struct kw_conf, kw


struct state_change
{
    int change_needed;
    int new_state;
};


struct state_change tokque_putthrough(FILE *yyout,
    char *text, size_t len, const struct kw_conf *s);
void tokque_finish_out(FILE *yyout);


#endif
