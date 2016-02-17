#include <lib_fsqlf.h>


void fsqlf_kwall_set_case(enum fsqlf_kwcase keyword_case)
{
    #define XMACRO( NAME , ... )   \
        fsqlf_kw_get(#NAME)->print_case = keyword_case;
    #include "kw_defaults.def"
    #undef XMACRO
}


// Set used variation of keyword text. (e.g. "left outer join" vs "left join")
void fsqlf_kwall_set_spelling(enum fsqlf_kwspelling kw_text_to_use)
{
    #define XMACRO( NAME , ... )           \
        fsqlf_kw_get(#NAME)->print_original_text = kw_text_to_use;
    #include "kw_defaults.def"
    #undef XMACRO
}


// Init all keyword settings to defaults.
void fsqlf_kwall_init(struct fsqlf_kw_conf * (*fsqlf_kw_get)(const char *))
{
    #define XMACRO(NAME, gib, nlb, tb, sb, gia, nla, ta, sa, TEXT)    \
    do {                                                        \
        FSQLF_kw_create(#NAME);                                 \
        fsqlf_kw_get(#NAME)->before.global_indent_change    = gib; \
        fsqlf_kw_get(#NAME)->before.new_line    = nlb;          \
        fsqlf_kw_get(#NAME)->before.indent      = tb;           \
        fsqlf_kw_get(#NAME)->before.space       = sb;           \
        fsqlf_kw_get(#NAME)->after.global_indent_change     = gia; \
        fsqlf_kw_get(#NAME)->after.new_line     = nla;          \
        fsqlf_kw_get(#NAME)->after.indent       = ta;           \
        fsqlf_kw_get(#NAME)->after.space        = sa;           \
        fsqlf_kw_get(#NAME)->print_original_text = FSQLF_KWSPELLING_USE_HARDCODED_DEFAULT; \
        fsqlf_kw_get(#NAME)->print_case         = FSQLF_KWCASE_UPPER; \
        fsqlf_kw_get(#NAME)->text               = TEXT;         \
        fsqlf_kw_get(#NAME)->is_word            = 1;            \
    } while (0);
    #include "kw_defaults.def"
    #undef XMACRO
}
