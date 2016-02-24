#include <lib_fsqlf.h>


void fsqlf_kwmap_set_case(struct fsqlf_kw_conf *kwall, enum fsqlf_kwcase keyword_case)
{
    #define XMACRO( NAME , ... )   \
        fsqlf_kw_get(kwall, #NAME)->print_case = keyword_case;
    #include "kw_defaults.def"
    #undef XMACRO
}


// Set used variation of keyword text. (e.g. "left outer join" vs "left join")
void fsqlf_kwmap_set_spelling(struct fsqlf_kw_conf *kwall, enum fsqlf_kwspelling kw_text_to_use)
{
    #define XMACRO( NAME , ... )           \
        fsqlf_kw_get(kwall, #NAME)->print_original_text = kw_text_to_use;
    #include "kw_defaults.def"
    #undef XMACRO
}


// Init all keyword settings to defaults.
void fsqlf_kwmap_init(struct fsqlf_kw_conf **kwall)
{
    *kwall = NULL;
    #define XMACRO(NAME, gib, nlb, tb, sb, gia, nla, ta, sa, TEXT) \
    do { \
        FSQLF_kw_create(kwall, #NAME); \
        fsqlf_kw_get(*kwall, #NAME)->before.global_indent_change = gib; \
        fsqlf_kw_get(*kwall, #NAME)->before.new_line    = nlb; \
        fsqlf_kw_get(*kwall, #NAME)->before.indent      = tb; \
        fsqlf_kw_get(*kwall, #NAME)->before.space       = sb; \
        fsqlf_kw_get(*kwall, #NAME)->after.global_indent_change = gia; \
        fsqlf_kw_get(*kwall, #NAME)->after.new_line     = nla; \
        fsqlf_kw_get(*kwall, #NAME)->after.indent       = ta; \
        fsqlf_kw_get(*kwall, #NAME)->after.space        = sa; \
        fsqlf_kw_get(*kwall, #NAME)->print_original_text = FSQLF_KWSPELLING_USE_HARDCODED_DEFAULT; \
        fsqlf_kw_get(*kwall, #NAME)->print_case         = FSQLF_KWCASE_UPPER; \
        fsqlf_kw_get(*kwall, #NAME)->text               = TEXT; \
        fsqlf_kw_get(*kwall, #NAME)->is_word            = 1;  \
    } while (0);
    #include "kw_defaults.def"
    #undef XMACRO
}
