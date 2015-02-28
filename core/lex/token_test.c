#include <stdio.h>
#include "token.h"


void token_demo()
{
    printf("Demo token.h\n");
    struct token *t = make_token(TC_CMT, "blabla", 6, NULL);
    printf("t{%d, %s, %d, %d}\n"
        , t->token_class, t->text, t->leng, t->kw_setting);
}


int main()
{
    token_demo();
    return 0;
}
