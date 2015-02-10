#ifndef CLI_H
#define CLI_H



// Without following typedef, error is issued:
// core/cli.h:12:53: warning: 'struct kw_conf' declared inside parameter list [enabled by default]
// GCC bug?
typedef struct kw_conf* some_random_name_works_as_workaround;

void read_cli_options(int argc, char **argv,
	struct kw_conf * (*kw)(const char *));


#endif
