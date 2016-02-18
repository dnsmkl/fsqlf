#ifndef CLI_H
#define CLI_H


#include <stdio.h> // fprintf, FILE
#include <lib_fsqlf.h>


void read_cli_options(struct fsqlf_kw_conf *kwall, int argc, char **argv,
    FILE **fin, FILE **fout);


#endif
