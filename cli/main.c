#include <lib_fsqlf.h>
#include "cli.h"    // read_cli_options


int main(int argc, char **argv)
{
    // Initialise with STD I/O (later can be changed by command line options).
    FILE *fin, *fout;
    fin = stdin;
    fout = stdout;

    fsqlf_kwall_init(&fsqlf_kw_get);             // Init default configs.
    fsqlf_kwconffile_read_default(&fsqlf_kw_get);        // Read configs from file.

    read_cli_options(argc, argv, &fsqlf_kw_get, &fin, &fout);  // Read configs from command line.
    fsqlf_format_file(fin, fout);

    fsqlf_kwall_delete();

    return 0;
}
