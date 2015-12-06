#include <lib_fsqlf.h>
#include "formatter/lex.yy.h"


void fsqlf_set_file_in(FILE *in)
{
    yyin = in;
}


void fsqlf_set_file_out(FILE *out)
{
    yyout = out;
}
