#ifndef token_h
#define token_h


#include "kw/kw.h"  // kw_conf


typedef int token_class_t;
#define TC_KW 1
#define TC_SP 2
#define TC_CMT 3


typedef struct
{
    token_class_t token_class;
    const char * yytext;
    int yyleng;
    const struct kw_conf *kw_setting;
} token_t;


token_t * make_token(
    const token_class_t token_class,
    const char * yytext,
    const int yyleng,
    const struct kw_conf *kw_setting);


#endif
