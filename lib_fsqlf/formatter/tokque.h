#ifndef tokque_h
#define tokque_h


// struct fsqlf_kw_conf, fsqlf_kw_get, struct FSQLF_prev_kw
#include <lib_fsqlf.h>
#include "../../utils/queue/queue.h"


struct FSQLF_state_change
{
    int state_change_action;
    int new_state;      // start conditions of flex lexer generator
};


// SCA - state change action
enum FSQLF_state_change_action
{
    FSQLF_SCA_NONE,
    FSQLF_SCA_BEGIN,
    FSQLF_SCA_PUSH,
    FSQLF_SCA_POP
};


void FSQLF_tokque_init(struct FSQLF_queue *tq);



struct FSQLF_state_change FSQLF_tokque_putthrough(
    struct FSQLF_queue *tq,
    FILE *fout,
    struct FSQLF_out_buffer *bout,
    int *currindent,
    char *text,
    size_t len,
    struct fsqlf_kw_conf *s,
    int cur_state
);


void FSQLF_tokque_finish_out(struct FSQLF_queue *tq, FILE *fout, struct FSQLF_out_buffer *bout);


#endif
