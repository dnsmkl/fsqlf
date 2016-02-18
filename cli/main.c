#include <lib_fsqlf.h>
#include "cli.h"    // read_cli_options


int main(int argc, char **argv)
{
    // Initialise with STD I/O (later can be changed by command line options).
    FILE *fin, *fout;
    fin = stdin;
    fout = stdout;
    struct fsqlf_kw_conf *kwall = NULL;

    fsqlf_kwall_init(&kwall);             // Init default configs.
    fsqlf_kwconffile_read_default(kwall);        // Read configs from file.
    read_cli_options(kwall, argc, argv, &fin, &fout);  // Read configs from command line.

    fsqlf_format_file(kwall, fin, fout);

    fsqlf_kwall_delete(kwall);

    return 0;
}
