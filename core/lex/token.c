#include <stdlib.h>   // malloc
#include "token.h"


token_t * make_token(
    const token_class_t token_class,
    const char * yytext,
    const int yyleng,
    const struct kw_conf *kw_setting)
{
    token_t *token;
    token = malloc(sizeof(token_t));
    token->token_class = token_class;
    // TODO: Allocate space for string and copy it.
    token->yytext = yytext;
    token->yyleng = yyleng;
    token->kw_setting = kw_setting;
    return token;
}
