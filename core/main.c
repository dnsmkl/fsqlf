#include "conf_file/conf_file_read.h"   // read_default_conf_file
#include "kw/kwall_init.h"  // init_all_settings
#include "kw/kw.h"  // init_all_settings
#include "cli.h"    // read_cli_options
#include "formatter/lex.yy.h" // yyin, yyout


int main(int argc, char **argv)
{
    // Initialise with STD I/O (later can be changed by command line options).
    yyin  = stdin;
    yyout = stdout;

    init_all_settings(&kw);             // Init default configs.
    read_default_conf_file(&kw);        // Read configs from file.
    read_cli_options(argc, argv, &kw, &yyin, &yyout);  // Read configs from command line.

    while (yylex() != 0) ;

    return 0;
}
