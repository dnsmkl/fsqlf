#ifndef print_keywords_h
#define print_keywords_h


#include <lib_fsqlf.h> // struct fsqlf_kw_conf, fsqlf_kw_get


void FSQLF_print(
    FILE *fout,
    size_t indent,
    const char *text,
    const struct fsqlf_kw_conf *kw
);


#endif
