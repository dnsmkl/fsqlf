#ifndef print_keywords_h
#define print_keywords_h


#include "../kw/kw.h" // struct kw_conf, kw


void use_token(FILE *yyout, char *yytext, const struct kw_conf *s);


#endif
