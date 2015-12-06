#include <lib_fsqlf.h>
#include "cli.h"    // read_cli_options
#include "../lib_fsqlf/formatter/lex.yy.h"    // yylex


int main(int argc, char **argv)
{
    // Initialise with STD I/O (later can be changed by command line options).
    FILE *fin, *fout;
    fin = stdin;
    fout = stdout;

    init_all_settings(&kw);             // Init default configs.
    read_default_conf_file(&kw);        // Read configs from file.

    read_cli_options(argc, argv, &kw, &fin, &fout);  // Read configs from command line.
    fsqlf_set_file_in(fin);
    fsqlf_set_file_out(fout);

    while (yylex() != 0) ;

    kw_delete_all();

    return 0;
}
