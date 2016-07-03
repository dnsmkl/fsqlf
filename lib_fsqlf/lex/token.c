#include <stdlib.h>   // malloc
#include <assert.h>   // assert
#include <string.h>   // strncpy
#include "token.h"


struct FSQLF_token *FSQLF_make_token(
    const enum FSQLF_token_class token_class,
    const char *text,
    const int leng,
    struct fsqlf_kw_conf *kw_setting,
    const size_t indent)
{
    struct FSQLF_token *tok;
    tok = malloc(sizeof(struct FSQLF_token));
    assert(tok);
    FSQLF_set_token(tok, token_class, text, leng, kw_setting, indent);
    return tok;
}


void FSQLF_delete_token(struct FSQLF_token **tok)
{
    FSQLF_clear_token(*tok);
    free((*tok));
    (*tok) = NULL;
}


void FSQLF_set_token(struct FSQLF_token * tok,
    const enum FSQLF_token_class token_class,
    const char *text,
    const int leng,
    struct fsqlf_kw_conf *kw_conf,
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


void FSQLF_clear_token(struct FSQLF_token *tok)
{
    free(tok->text);
    tok->text = NULL;
}
