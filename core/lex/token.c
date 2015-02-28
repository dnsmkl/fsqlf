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
    struct token *tok;
    tok = malloc(sizeof(struct token));
    assert(tok);
    tok->token_class = token_class;

    tok->text = malloc(leng+1);
    assert(tok->text);
    strncpy(tok->text, text, leng);
    tok->text[leng] = '\0';
    tok->leng = leng;

    tok->kw_setting = kw_setting;
    return tok;
}


void delete_token(struct token **tok)
{
    free((*tok)->text);
    (*tok)->text = NULL;
    free((*tok));
    (*tok) = NULL;
}
