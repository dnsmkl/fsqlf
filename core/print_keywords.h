#ifndef print_keywords_h
#define print_keywords_h


#include "kw/kw.h" // struct kw_conf, kw


void handle_kw(FILE *yyout, char *yytext, struct kw_conf *s);
void handle_text(FILE *yyout, char *txt);


#endif
