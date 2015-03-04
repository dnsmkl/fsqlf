#include <stdlib.h>   // malloc
#include <assert.h>   // assert
#include <string.h>   // strncpy
#include "token.h"


struct token *make_token(
    const token_class_t token_class,
    const char *text,
    const int leng,
    const struct kw_conf *kw_setting,
    const size_t indent)
{
    struct token *tok;
    tok = malloc(sizeof(struct token));
    assert(tok);
    set_token(tok, token_class, text, leng, kw_setting, indent);
    return tok;
}


void delete_token(struct token **tok)
{
    clear_token(*tok);
    free((*tok));
    (*tok) = NULL;
}


void set_token(struct token * tok,
    const token_class_t token_class,
    const char *text,
    const int leng,
    const struct kw_conf *kw_conf,
    const size_t indent)
{
    assert(tok);
    tok->token_class = token_class;

    tok->text = malloc(leng+1);
    assert(tok->text);
    strncpy(tok->text, text, leng);
    tok->text[leng] = '\0';
    tok->leng = leng;

    tok->kw_setting = kw_conf;
    tok->indent = indent;
}


void clear_token(struct token *tok)
{
    free(tok->text);
    tok->text = NULL;
}
