#ifndef print_keywords_h
#define print_keywords_h


#include "../kw/kw.h" // struct kw_conf, kw


void tokque_putthrough(FILE *yyout, char *text, size_t len, const struct kw_conf *s);
void tokque_finish_out(FILE *yyout);


#endif
