#include "lex.yy.h"
#include <lib_fsqlf.h>


int fsqlf_format_bytes(const char *bytes, int len)
{
    int ret;
    yyscan_t scanner;
    yy_scan_bytes(bytes, len, scanner);
    yylex_init(&scanner);
    yylex(scanner);
    yylex_destroy(scanner);
    return ret;
}


int fsqlf_format_file(FILE *fin, FILE *fout)
{
    int ret;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyset_in(fin, scanner);
    yyset_out(fout, scanner);
    yylex(scanner);
    yylex_destroy(scanner);
    return ret;
}
