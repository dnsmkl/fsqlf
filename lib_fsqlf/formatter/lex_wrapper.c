#include "lex.yy.h"
#include <lib_fsqlf.h>


void fsqlf_format_bytes(fsqlf_kwmap_t kwmap,
    const char *bytes_in, int len, char **bytes_out
)
{
    struct fsqlf_formatter_state f_state;
    FSQLF_stack_init(&f_state.lexstate_stack, sizeof(int));
    FSQLF_stack_init(&f_state.sub_openings, sizeof(pair));
    FSQLF_tokque_init(&f_state.tqueue);
    f_state.currindent = 0;
    f_state.left_p = 0;
    f_state.right_p = 0;
    f_state.kwall = kwmap;
    f_state.bout.len_used = 0;
    f_state.bout.len_alloc = len * 1.5;
    f_state.bout.buffer = malloc(f_state.bout.len_alloc);

    yyscan_t scanner;
    yylex_init(&scanner);
    yy_scan_bytes(bytes_in, len, scanner);
    yyset_extra(&f_state, scanner);

    yylex(scanner);
    *bytes_out = f_state.bout.buffer;

    yylex_destroy(scanner);
}


void fsqlf_format_file(struct fsqlf_kw_conf *kwall, FILE *fin, FILE *fout)
{
    struct fsqlf_formatter_state f_state;
    FSQLF_stack_init(&f_state.lexstate_stack, sizeof(int));
    FSQLF_stack_init(&f_state.sub_openings, sizeof(pair));
    FSQLF_tokque_init(&f_state.tqueue);
    f_state.currindent = 0;
    f_state.left_p = 0;
    f_state.right_p = 0;
    f_state.kwall = kwall;
    f_state.bout = (struct FSQLF_out_buffer){NULL, 0, 0};

    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(fin, scanner);
    yyset_out(fout, scanner);
    yyset_extra(&f_state, scanner);

    yylex(scanner);

    yylex_destroy(scanner);
}
