#include <stdio.h>      // fprintf, fputs
#include "globals.h"
#include "../../utils/queue/queue.h"
#include "../lex/token.h"
#include "print_keywords.h" // echo_print, kw_print


static int tokque_print_one(struct queue * tokque_ptr, FILE *yyout)
{
    if (!queue_empty(tokque_ptr)) {
        struct token *tok2 = (struct token *) queue_peek_n(tokque_ptr, 0);
        if (tok2->kw_setting == NULL) {
            echo_print(yyout, tok2->indent, tok2->text);
        } else {
            kw_print(yyout, tok2->indent, tok2->text, *(tok2->kw_setting));
        }
        queue_drop_head(tokque_ptr);
        clear_token(tok2);
        return 1; // success - printing was made
    } else {
        return 0; // queue was empty, so printing was not possible
    }
}


struct queue tokque; // GLOBAL


// This routine has goal to do 4 things:
// 1. Place token on the queue.
// 2. Accumulate tokens till enough info is available for recognition.
// 3. Perform recognition.
// 4. Output fully recognized tokens.
///
// At the moment only 1st and 4th parts are done.
// TODO: implement 2nd and 3rd
void tokque_putthrough(FILE *yyout, char *text, size_t len, const struct kw_conf *s)
{
    // Queue initialization.
    static int first_run = 1;
    if (first_run) {
        first_run = 0;
        queue_init(&tokque, sizeof(struct token));
    }

    // Place on queue.
    {
        struct token *tok1 = (struct token *) queue_alloc_back(&tokque);
        set_token(tok1, 0, text, len, s, currindent);
    }

    // Retrieve from queue and print.
    tokque_print_one(&tokque, yyout);
}


void tokque_finish_out(FILE *yyout)
{
    // Print out all queue
    while (tokque_print_one(&tokque, yyout)) {}
    // Cleanup
    queue_clear(&tokque);
}
