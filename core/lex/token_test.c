#include <stdio.h>
#include "token.h"


void token_demo()
{
    printf("Demo token.h\n");
    token_t * t = make_token(TC_CMT, "blabla", 6, NULL);
    printf("t{%d, %s, %d, %d}\n"
        , t->token_class, t->yytext, t->yyleng, t->kw_setting);
}


int main()
{
    token_demo();
    return 0;
}
