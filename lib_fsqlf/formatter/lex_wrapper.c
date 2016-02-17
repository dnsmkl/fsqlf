#include "lex.yy.h"
#include <lib_fsqlf.h>


int fsqlf_format_bytes(const char *bytes, int len)
{
    int ret=0; // TODO: change return type. Currently unused!
    yyscan_t scanner;
    yy_scan_bytes(bytes, len, scanner);
    yylex_init(&scanner);
    yylex(scanner);
    yylex_destroy(scanner);
    return ret;
}


void fsqlf_format_file(FILE *fin, FILE *fout)
{
    struct fsqlf_formatter_state f_state;
    FSQLF_stack_init(&f_state.lexstate_stack, sizeof(int));
    FSQLF_stack_init(&f_state.sub_openings, sizeof(pair));
    f_state.currindent = 0;
    f_state.left_p = 0;
    f_state.right_p = 0;

    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(fin, scanner);
    yyset_out(fout, scanner);
    yyset_extra(&f_state, scanner);

    yylex(scanner);

    yylex_destroy(scanner);
}
