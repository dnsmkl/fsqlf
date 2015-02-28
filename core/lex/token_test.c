#include <string.h> // memcmp
#include <assert.h> // assert
#include "token.h"


void token_test()
{
    struct token *t = make_token(TC_CMT, "blabla", 6, NULL);

    assert(t->token_class == TC_CMT);
    assert(t->leng == 6);
    assert(memcmp(t->text, "blabla", t->leng) == 0);
    assert(t->kw_setting == NULL);

    delete_token(&t);
    assert(!t);
}


int main()
{
    token_test();
    return 0;
}
