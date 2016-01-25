#ifndef CLI_H
#define CLI_H


#include <stdio.h> // fprintf, FILE
#include <lib_fsqlf.h>


void read_cli_options(int argc, char **argv,
    struct fsqlf_kw_conf * (*fsqlf_kw_get)(const char *), FILE ** fin, FILE ** fout);


#endif
