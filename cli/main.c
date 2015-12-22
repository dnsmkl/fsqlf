#include <lib_fsqlf.h>
#include "cli.h"    // read_cli_options
#include "../lib_fsqlf/formatter/lex.yy.h"    // yylex


int main(int argc, char **argv)
{
    // Initialise with STD I/O (later can be changed by command line options).
    FILE *fin, *fout;
    fin = stdin;
    fout = stdout;

    fsqlf_init_all_kw(&kw);             // Init default configs.
    fsqlf_read_default_conf_file(&kw);        // Read configs from file.

    read_cli_options(argc, argv, &kw, &fin, &fout);  // Read configs from command line.
    fsqlf_set_file_in(fin);
    fsqlf_set_file_out(fout);

    yylex(); // run 'lexer' which handles the formatting.

    kw_delete_all();

    return 0;
}
