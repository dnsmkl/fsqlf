#include <lib_fsqlf.h>


void fsqlf_set_all_kwcase(enum fsqlf_kwcase keyword_case)
{
    #define XMACRO( NAME , ... )   \
        kw(#NAME)->print_case = keyword_case;
    #include "kw_defaults.def"
    #undef XMACRO
}


// Set used variation of keyword text. (e.g. "left outer join" vs "left join")
void fsqlf_set_all_kwvariant(enum fsqlf_kwtext kw_text_to_use)
{
    #define XMACRO( NAME , ... )           \
        kw(#NAME)->print_original_text = kw_text_to_use;
    #include "kw_defaults.def"
    #undef XMACRO
}


void init_all_settings(struct kw_conf * (*kw)(const char *))
{
    #define XMACRO(NAME, gib, nlb, tb, sb, gia, nla, ta, sa, TEXT)    \
    do {                                              \
        kw_add(#NAME);                                \
        kw(#NAME)->before.global_indent_change    = gib; \
        kw(#NAME)->before.new_line    = nlb;          \
        kw(#NAME)->before.indent      = tb;           \
        kw(#NAME)->before.space       = sb;           \
        kw(#NAME)->after.global_indent_change     = gia; \
        kw(#NAME)->after.new_line     = nla;          \
        kw(#NAME)->after.indent       = ta;           \
        kw(#NAME)->after.space        = sa;           \
        kw(#NAME)->print_original_text = FSQLF_KWTEXT_USE_HARDCODED_DEFAULT; \
        kw(#NAME)->print_case         = FSQLF_KWCASE_UPPER; \
        kw(#NAME)->text               = TEXT;         \
        kw(#NAME)->is_word            = 1;            \
        kw(#NAME)->funct_before[0] = NULL;   \
        kw(#NAME)->funct_before[1] = NULL;   \
        kw(#NAME)->funct_before[2] = NULL;   \
    } while (0);
    #include "kw_defaults.def"
    #undef XMACRO

    #define XMACRO(NAME, fb1, fb2, fb3) \
    do {                                    \
        kw(#NAME)->funct_before[0] = fb1;   \
        kw(#NAME)->funct_before[1] = fb2;   \
        kw(#NAME)->funct_before[2] = fb3;   \
    } while (0);
    #include "kw_callbacks.def"
    #undef XMACRO

}
