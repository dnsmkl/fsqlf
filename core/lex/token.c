#include "token.h"


token_t * make_token(
    const token_class_t token_class,
    const char * yytext,
    const int yyleng,
    const struct kw_conf *kw_setting)
{
    // TODO: Allocate new token.
    // Currently just definining interface.
    static token_t token;
    token.token_class = token_class;
    // TODO: Allocate space for string and copy it.
    token.yytext = yytext;
    token.yyleng = yyleng;
    token.kw_setting = kw_setting;
    return &token;
}
