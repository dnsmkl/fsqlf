#include <lib_fsqlf.h>
#include "cli.h"    // read_cli_options


int main(int argc, char **argv)
{
    // Initialise with STD I/O (later can be changed by command line options).
    FILE *fin, *fout;
    fin = stdin;
    fout = stdout;
    fsqlf_kwmap_t kwmap;

    fsqlf_kwmap_init(&kwmap);             // Init default configs.
    fsqlf_kwmap_conffile_read_default(kwmap);        // Read configs from file.
    read_cli_options(kwmap, argc, argv, &fin, &fout);  // Read configs from command line.

    fsqlf_format_file(kwmap, fin, fout);

    fsqlf_kwmap_destroy(kwmap);

    return 0;
}
