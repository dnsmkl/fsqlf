#include <stdlib.h>   // malloc
#include <assert.h>   // assert
#include <string.h>   // strncpy
#include "token.h"


struct token *make_token(
    const token_class_t token_class,
    const char *text,
    const int leng,
    const struct kw_conf *kw_setting)
{
    struct token *token;
    token = malloc(sizeof(struct token));
    assert(token);
    token->token_class = token_class;

    token->text = malloc(leng+1);
    strncpy(token->text, text, leng);
    token->leng = leng;

    token->kw_setting = kw_setting;
    return token;
}
