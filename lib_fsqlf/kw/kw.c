#include <stdlib.h>  // malloc
#include <assert.h>  // assert
#include <lib_fsqlf.h> // struct fsqlf_kw_conf


// Global map variable. should not be accessed directly.
struct fsqlf_kw_conf *g_keyword_config = NULL;


void FSQLF_kw_create(const char *name)
{
    struct fsqlf_kw_conf *tmp;
    tmp = (struct fsqlf_kw_conf*) calloc(1, sizeof(struct fsqlf_kw_conf));
    assert(tmp);
    tmp->name = name;
    HASH_ADD_KEYPTR(hh, g_keyword_config, tmp->name, strlen(tmp->name), tmp);
}


struct fsqlf_kw_conf *fsqlf_kw_get(const char *name)
{
    struct fsqlf_kw_conf *match;
    HASH_FIND_STR(g_keyword_config, name, match);
    return match;
}


void fsqlf_kwall_delete()
{
    struct fsqlf_kw_conf *current_kw, *tmp;
    HASH_ITER(hh, g_keyword_config, current_kw, tmp) {
        HASH_DEL(g_keyword_config, current_kw);
        free(current_kw);
    }
}
