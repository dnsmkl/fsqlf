#include "lib_fsqlf.h"


void fsqlf_set_file_in(FILE *in)
{
    yyin = in;
}


void fsqlf_set_file_out(FILE *out)
{
    yyout = out;
}
