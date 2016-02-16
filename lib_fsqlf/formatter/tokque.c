#include <stdio.h>      // fprintf, fputs
#include "globals.h"
#include "../../utils/queue/queue.h"
#include "../lex/token.h" // struct FSQLF_token, FSQLF_clear_token, FSQLF_set_token
#include "print_keywords.h" // FSQLF_echo_print, FSQLF_kw_print
#include "tokque.h"
#define YY_HEADER_EXPORT_START_CONDITIONS
#include "lex.yy.h" // start conditions (states)


static int tokque_print_one(struct FSQLF_queue *tokque_ptr, FILE *fout)
{
    if (!FSQLF_queue_empty(tokque_ptr)) {
        struct FSQLF_token *tok2 = (struct FSQLF_token *) FSQLF_queue_peek_n(tokque_ptr, 0);
        if (tok2->kw_setting == NULL) {
            FSQLF_echo_print(fout, tok2->indent, tok2->text);
        } else {
            FSQLF_kw_print(fout, tok2->indent, tok2->text, *(tok2->kw_setting));
        }
        FSQLF_queue_drop_head(tokque_ptr);
        FSQLF_clear_token(tok2);
        return 1; // success - printing was made
    } else {
        return 0; // queue was empty, so printing was not possible
    }
}


static struct FSQLF_state_change decide_new_state(
    int cur_state,
    const struct fsqlf_kw_conf *s
)
{
    if (s == fsqlf_kw_get("kw_deletefrom")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stDELETE};
    else if (s == fsqlf_kw_get("kw_insertinto")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stINSERT};
    else if (s == fsqlf_kw_get("kw_update")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stUPDATE};
    else if (s == fsqlf_kw_get("kw_create")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stCREATE};
    else if (s == fsqlf_kw_get("kw_drop")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_ifexists")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_view")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_union")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_union_all")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_minus")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_intersect")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_except")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_semicolon")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
    else if (s == fsqlf_kw_get("kw_groupby")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stGROUPBY};
    else if (s == fsqlf_kw_get("kw_orderby")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stORDERBY};
    else if (s == fsqlf_kw_get("kw_having")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stWHERE};
    else if (s == fsqlf_kw_get("kw_qualify")) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stWHERE};

    return (struct FSQLF_state_change) {0, 0};
}


struct FSQLF_queue FSQLF_tokque; // GLOBAL


// This routine has goal to do 4 things:
// 1. Place token on the queue.
// 2. Accumulate tokens till enough info is available for recognition.
// 3. Perform recognition.
// 4. Output fully recognized tokens.
///
// At the moment only 1st and 4th parts are done.
// TODO: implement 2nd and 3rd
struct FSQLF_state_change FSQLF_tokque_putthrough(FILE *fout, int *currindent,
    char *text, size_t len, const struct fsqlf_kw_conf *s, int cur_state)
{
    // Queue initialization.
    static int first_run = 1;
    if (first_run) {
        first_run = 0;
        FSQLF_queue_init(&FSQLF_tokque, sizeof(struct FSQLF_token));
    }

    // Place on queue.
    {
        struct FSQLF_token *tok1 =
            (struct FSQLF_token *) FSQLF_queue_alloc_back(&FSQLF_tokque);
        if (s) (*currindent) += s->before.global_indent_change;
        FSQLF_set_token(tok1, 0, text, len, s, (*currindent));
        if (s) (*currindent) += s->after.global_indent_change;
    }

    // Retrieve from queue and print.
    tokque_print_one(&FSQLF_tokque, fout);

    // Send command for state change
    return decide_new_state(cur_state, s);
}


void FSQLF_tokque_finish_out(FILE *fout)
{
    // Print out all queue
    while (tokque_print_one(&FSQLF_tokque, fout)) {}
    // Cleanup
    FSQLF_queue_clear(&FSQLF_tokque);
}
