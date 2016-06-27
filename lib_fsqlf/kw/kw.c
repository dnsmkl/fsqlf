#include <stdlib.h>  // malloc
#include <assert.h>  // assert
#include <lib_fsqlf.h> // struct fsqlf_kw_conf


// Functions that retrieve single element of kw-map.


void FSQLF_kw_create(fsqlf_kwmap_t *kwmap,  const char *name)
{
    struct fsqlf_kw_conf *tmp;
    tmp = (struct fsqlf_kw_conf*) calloc(1, sizeof(struct fsqlf_kw_conf));
    assert(tmp);
    tmp->name = name;
    HASH_ADD_KEYPTR(hh, *kwmap, tmp->name, strlen(tmp->name), tmp);
}


struct fsqlf_kw_conf *fsqlf_kw_get(fsqlf_kwmap_t kwmap, const char *name)
{
    struct fsqlf_kw_conf *match;
    HASH_FIND_STR(kwmap, name, match);
    return match;
}
