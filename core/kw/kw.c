#include <stdlib.h>  // malloc
#include <assert.h>  // assert
#include "kw.h" // struct kw_conf


// Global map variable. should not be accessed directly.
struct kw_conf *g_keyword_config = NULL;


struct kw_conf *kw_add(const char *name)
{
    struct kw_conf *tmp;
    tmp = (struct kw_conf*) malloc(sizeof(struct kw_conf));
    assert(tmp);
    tmp->name = name;
    HASH_ADD_KEYPTR(hh, g_keyword_config, tmp->name, strlen(tmp->name), tmp);
    return tmp;
}


struct kw_conf *kw(const char *name)
{
    struct kw_conf *match;
    HASH_FIND_STR(g_keyword_config, name, match);
    return match;
}
