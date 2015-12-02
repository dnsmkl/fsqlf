#include "../lib_fsqlf/conf_file/conf_file_read.h"   // read_default_conf_file
#include "../lib_fsqlf/kw/kwall_init.h"  // init_all_settings
#include "../lib_fsqlf/kw/kw.h"  // kw()
#include "../lib_fsqlf/lib_fsqlf.h"    // fsqlf_set_file_in fsqlf_set_file_out
#include "cli.h"    // read_cli_options


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
