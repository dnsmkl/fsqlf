#include "conf_file/conf_file_read.h"   // read_default_conf_file
#include "kw/kwall_init.h"  // init_all_settings
#include "kw/kw.h"  // init_all_settings
#include "cli.h"    // read_cli_options
#include "lex.yy.h"	// yyin, yyout


// TODO: Make formatter lib separate from cli.
// 		 Extract function  "void format_sql_file(FILE in, FILE out, conf)"
// Prereq: change cli.c, so it would not access yyin, yyout directly.
int main(int argc, char **argv)
{
    // Initialise with STD I/O (later can be changed by command line options).
    yyin  = stdin;
    yyout = stdout;

    init_all_settings(&kw);        		// Init default configs.
    read_default_conf_file(&kw);       // Read configs from file.
    read_cli_options(argc, argv, &kw);   // Read configs from command line.

    while (yylex() != 0) ;

    return 0;
}
