#include <string.h> // memcmp
#include <assert.h> // assert
#include "token.h"


void token_test()
{
    struct token *t = FSQLF_make_token(FSQLF_TOKEN_CLASS_CMT, "blabla", 6, NULL);

    assert(t->token_class == FSQLF_TOKEN_CLASS_CMT);
    assert(t->leng == 6);
    assert(memcmp(t->text, "blabla", t->leng) == 0);
    assert(t->kw_setting == NULL);

    FSQLF_delete_token(&t);
    assert(!t);
}


int main()
{
    token_test();
    return 0;
}
