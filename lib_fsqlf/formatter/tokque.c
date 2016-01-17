#include <stdio.h>      // fprintf, fputs
#include "globals.h"
#include "../../utils/queue/queue.h"
#include "../lex/token.h"
#include "print_keywords.h" // echo_print, kw_print
#include "tokque.h"
#define YY_HEADER_EXPORT_START_CONDITIONS
#include "lex.yy.h" // start conditions (states)


static int tokque_print_one(struct FSQLF_queue * tokque_ptr, FILE *yyout)
{
    if (!FSQLF_queue_empty(tokque_ptr)) {
        struct token *tok2 = (struct token *) FSQLF_queue_peek_n(tokque_ptr, 0);
        if (tok2->kw_setting == NULL) {
            echo_print(yyout, tok2->indent, tok2->text);
        } else {
            kw_print(yyout, tok2->indent, tok2->text, *(tok2->kw_setting));
        }
        FSQLF_queue_drop_head(tokque_ptr);
        clear_token(tok2);
        return 1; // success - printing was made
    } else {
        return 0; // queue was empty, so printing was not possible
    }
}


struct state_change decide_new_state(int cur_state, const struct kw_conf *s)
{
    if (s == kw("kw_deletefrom")) return (struct state_change) {SCA_BEGIN, stDELETE};
    else if (s == kw("kw_insertinto")) return (struct state_change) {SCA_BEGIN, stINSERT};
    else if (s == kw("kw_update")) return (struct state_change) {SCA_BEGIN, stUPDATE};
    else if (s == kw("kw_create")) return (struct state_change) {SCA_BEGIN, stCREATE};
    else if (s == kw("kw_drop")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_ifexists")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_view")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_union")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_union_all")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_minus")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_intersect")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_except")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_semicolon")) return (struct state_change) {SCA_BEGIN, INITIAL};
    else if (s == kw("kw_groupby")) return (struct state_change) {SCA_BEGIN, stGROUPBY};
    else if (s == kw("kw_orderby")) return (struct state_change) {SCA_BEGIN, stORDERBY};
    else if (s == kw("kw_having")) return (struct state_change) {SCA_BEGIN, stWHERE};
    else if (s == kw("kw_qualify")) return (struct state_change) {SCA_BEGIN, stWHERE};

    return (struct state_change) {0, 0};
}


struct FSQLF_queue tokque; // GLOBAL


// This routine has goal to do 4 things:
// 1. Place token on the queue.
// 2. Accumulate tokens till enough info is available for recognition.
// 3. Perform recognition.
// 4. Output fully recognized tokens.
///
// At the moment only 1st and 4th parts are done.
// TODO: implement 2nd and 3rd
struct state_change tokque_putthrough(FILE *yyout, int *currindent,
    char *text, size_t len, const struct kw_conf *s, int cur_state)
{
    // Queue initialization.
    static int first_run = 1;
    if (first_run) {
        first_run = 0;
        FSQLF_queue_init(&tokque, sizeof(struct token));
    }

    // Place on queue.
    {
        struct token *tok1 = (struct token *) FSQLF_queue_alloc_back(&tokque);
        if (s) (*currindent) += s->before.global_indent_change;
        set_token(tok1, 0, text, len, s, (*currindent));
        if (s) (*currindent) += s->after.global_indent_change;
    }

    // Retrieve from queue and print.
    tokque_print_one(&tokque, yyout);

    // Send command for state change
    return decide_new_state(cur_state, s);
}


void tokque_finish_out(FILE *yyout)
{
    // Print out all queue
    while (tokque_print_one(&tokque, yyout)) {}
    // Cleanup
    FSQLF_queue_clear(&tokque);
}
