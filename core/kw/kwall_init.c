#include "kwall_init.h"


// TODO: remove these from here (use include instead)
extern void inc_LEFTP();
extern void inc_RIGHTP();
extern void begin_SUB();
extern void end_SUB();


void set_case(unsigned short int keyword_case)
{
    #define XMACRO( NAME , ... )   \
        kw(#NAME)->print_case = keyword_case;
    #include "kw_defaults.def"
    #undef XMACRO
}


void set_text_original(unsigned short int ind_original)
{
    #define XMACRO( NAME , ... )           \
        kw(#NAME)->print_original_text = ind_original;
    #include "kw_defaults.def"
    #undef XMACRO
}


void init_all_settings(struct kw_conf * (*kw)(const char *))
{
    #define XMACRO(NAME, nlb, tb, sb, nla, ta, sa, TEXT)    \
    do {                                              \
        kw_add(#NAME);                                \
        kw(#NAME)->before.new_line    = nlb;          \
        kw(#NAME)->before.indent      = tb;           \
        kw(#NAME)->before.space       = sb;           \
        kw(#NAME)->after.new_line     = nla;          \
        kw(#NAME)->after.indent       = ta;           \
        kw(#NAME)->after.space        = sa;           \
        kw(#NAME)->print_original_text = 0;           \
        kw(#NAME)->print_case         = CASE_UPPER;   \
        kw(#NAME)->text               = TEXT;         \
        kw(#NAME)->is_word            = 1;            \
    } while (0);
    #include "kw_defaults.def"
    #undef XMACRO

    #define XMACRO(NAME, fb1, fb2, fb3, fa1, fa2, fa3) \
    do {                                    \
        kw(#NAME)->funct_before[0] = fb1;   \
        kw(#NAME)->funct_before[1] = fb2;   \
        kw(#NAME)->funct_before[2] = fb3;   \
        kw(#NAME)->funct_after [0] = fa1;   \
        kw(#NAME)->funct_after [1] = fa2;   \
        kw(#NAME)->funct_after [2] = fa3;   \
    } while (0);
    #include "kw_callbacks.def"
    #undef XMACRO

}
