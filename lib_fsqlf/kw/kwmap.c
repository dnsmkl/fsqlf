#include <lib_fsqlf.h>


// Functions that operate on kw-map as a whole.
// (as opposed to manipulating single element of the map)


void fsqlf_kwmap_set_case(fsqlf_kwmap_t kwmap, enum fsqlf_kwcase keyword_case)
{
    #define XMACRO( NAME , ... )   \
        fsqlf_kw_get(kwmap, #NAME)->print_case = keyword_case;
    #include "kwmap_defaults.def"
    #undef XMACRO
}


// Set used variation of keyword text. (e.g. "left outer join" vs "left join")
void fsqlf_kwmap_set_spelling(fsqlf_kwmap_t kwmap, enum fsqlf_kwspelling kw_text_to_use)
{
    #define XMACRO( NAME , ... )           \
        fsqlf_kw_get(kwmap, #NAME)->print_original_text = kw_text_to_use;
    #include "kwmap_defaults.def"
    #undef XMACRO
}


// Init all keyword settings to defaults.
void fsqlf_kwmap_init(fsqlf_kwmap_t *kwmap)
{
    *kwmap = NULL;
    #define XMACRO(NAME, gib, nlb, tb, sb, gia, nla, ta, sa, TEXT) \
    do { \
        FSQLF_kw_create(kwmap, #NAME); \
        fsqlf_kw_get(*kwmap, #NAME)->before.global_indent_change = gib; \
        fsqlf_kw_get(*kwmap, #NAME)->before.new_line    = nlb; \
        fsqlf_kw_get(*kwmap, #NAME)->before.indent      = tb; \
        fsqlf_kw_get(*kwmap, #NAME)->before.space       = sb; \
        fsqlf_kw_get(*kwmap, #NAME)->after.global_indent_change = gia; \
        fsqlf_kw_get(*kwmap, #NAME)->after.new_line     = nla; \
        fsqlf_kw_get(*kwmap, #NAME)->after.indent       = ta; \
        fsqlf_kw_get(*kwmap, #NAME)->after.space        = sa; \
        fsqlf_kw_get(*kwmap, #NAME)->print_original_text = FSQLF_KWSPELLING_USE_HARDCODED_DEFAULT; \
        fsqlf_kw_get(*kwmap, #NAME)->print_case         = FSQLF_KWCASE_UPPER; \
        fsqlf_kw_get(*kwmap, #NAME)->text               = TEXT; \
        fsqlf_kw_get(*kwmap, #NAME)->is_word            = 1;  \
    } while (0);
    #include "kwmap_defaults.def"
    #undef XMACRO
}


void fsqlf_kwmap_destroy(fsqlf_kwmap_t kwmap)
{
    struct fsqlf_kw_conf *current_kw, *tmp;
    HASH_ITER(hh, kwmap, current_kw, tmp) {
        HASH_DEL(kwmap, current_kw);
        free(current_kw);
    }
}


void fsqlf_kwmap_set_major_clause_nl(fsqlf_kwmap_t kwmap, int nl_count)
{
    fsqlf_kw_get(kwmap, "kw_from")->before.new_line = nl_count;
    fsqlf_kw_get(kwmap, "kw_where")->before.new_line = nl_count;
    fsqlf_kw_get(kwmap, "kw_inner_join")->before.new_line = nl_count;
    fsqlf_kw_get(kwmap, "kw_left_join")->before.new_line  = nl_count;
    fsqlf_kw_get(kwmap, "kw_right_join")->before.new_line = nl_count;
    fsqlf_kw_get(kwmap, "kw_full_join")->before.new_line  = nl_count;
    fsqlf_kw_get(kwmap, "kw_cross_join")->before.new_line = nl_count;
}
