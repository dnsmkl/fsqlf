#include <stdio.h>      // fprintf, fputs
#include "../../utils/queue/queue.h"
#include "../lex/token.h" // struct FSQLF_token, FSQLF_clear_token, FSQLF_set_token
#include "print_keywords.h" // FSQLF_print
#include "tokque.h"
#define YY_HEADER_EXPORT_START_CONDITIONS
#include "lex.yy.h" // start conditions (states)


static int tokque_print_one(struct FSQLF_queue *tokque, FILE *fout,
    struct FSQLF_out_buffer *bout)
{
    if (!FSQLF_queue_empty(tokque)) {
        struct FSQLF_token *tok;
        tok = (struct FSQLF_token *) FSQLF_queue_peek_n(tokque, 0);
        FSQLF_print(fout, bout, tok->indent, tok->text, tok->kw_setting);
        FSQLF_queue_drop_head(tokque);
        FSQLF_clear_token(tok);
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
    if (s) {
        if (strcmp(s->name, "kw_deletefrom") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stDELETE};
        else if (strcmp(s->name, "kw_insertinto") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stINSERT};
        else if (strcmp(s->name, "kw_update") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stUPDATE};
        else if (strcmp(s->name, "kw_create") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stCREATE};
        else if (strcmp(s->name, "kw_drop") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_ifexists") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_view") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_union") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_union_all") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_minus") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_intersect") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_except") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_semicolon") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, INITIAL};
        else if (strcmp(s->name, "kw_groupby") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stGROUPBY};
        else if (strcmp(s->name, "kw_orderby") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stORDERBY};
        else if (strcmp(s->name, "kw_having") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stWHERE};
        else if (strcmp(s->name, "kw_qualify") == 0) return (struct FSQLF_state_change) {FSQLF_SCA_BEGIN, stWHERE};
    }

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
struct FSQLF_state_change FSQLF_tokque_putthrough(
    FILE *fout,
    struct FSQLF_out_buffer *bout,
    int *currindent,
    char *text,
    size_t len,
    const struct fsqlf_kw_conf *s,
    int cur_state
)
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
    tokque_print_one(&FSQLF_tokque, fout, bout);

    // Send command for state change
    return decide_new_state(cur_state, s);
}


void FSQLF_tokque_finish_out(FILE *fout, struct FSQLF_out_buffer *bout)
{
    // Print out all queue
    while (tokque_print_one(&FSQLF_tokque, fout, bout)) {}
    // Cleanup
    FSQLF_queue_clear(&FSQLF_tokque);
}
