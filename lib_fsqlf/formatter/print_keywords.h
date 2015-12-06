#ifndef print_keywords_h
#define print_keywords_h


#include <lib_fsqlf.h> // struct kw_conf, kw


void kw_print(FILE *yyout, size_t indent, const char *yytext,
                        struct kw_conf s);
void echo_print(FILE *yyout, size_t indent, char *txt);


#endif
