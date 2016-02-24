#include <stdlib.h>  // malloc
#include <assert.h>  // assert
#include <lib_fsqlf.h> // struct fsqlf_kw_conf


void FSQLF_kw_create(struct fsqlf_kw_conf **kwall,  const char *name)
{
    struct fsqlf_kw_conf *tmp;
    tmp = (struct fsqlf_kw_conf*) calloc(1, sizeof(struct fsqlf_kw_conf));
    assert(tmp);
    tmp->name = name;
    HASH_ADD_KEYPTR(hh, *kwall, tmp->name, strlen(tmp->name), tmp);
}


struct fsqlf_kw_conf *fsqlf_kw_get(struct fsqlf_kw_conf *kwall, const char *name)
{
    struct fsqlf_kw_conf *match;
    HASH_FIND_STR(kwall, name, match);
    return match;
}
