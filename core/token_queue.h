#ifndef token_queue_h
#define token_queue_h


#define KW_SETTING_T int // Temporary dummy definition replacement.
#include "token.h"
#include <stdio.h>




// TODO: Implement actual queue
// currently only interface is implemented,
// actualy hold only 1 element.


typedef struct
{
    token_t * token;
    size_t length;
} tqueue_t;


void tqueue_init(tqueue_t * q)
{
    q->length = 0;
}


void tqueue_push_back(tqueue_t * q, const token_t * token)
{
    q->token = (token_t*) token;
    q->length++;
}


void tqueue_drop_head(tqueue_t * q)
{
    if(q->length > 0) q->length--;
}


token_t * tqueue_peek_n(const tqueue_t * q, const size_t n)
{
    return q->token;
}


int tqueue_empty(const tqueue_t * q)
{
    return q->length == 0;
}


#endif
