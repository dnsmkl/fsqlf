#ifndef CLI_H
#define CLI_H


#include "kw/kw.h"


void read_cli_options(int argc, char **argv,
	struct kw_conf * (*kw)(const char *));


#endif
