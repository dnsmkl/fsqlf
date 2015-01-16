#include "kwall_init.h"


// TODO: remove these from here (use include instead)
extern void inc_LEFTP();
extern void inc_RIGHTP();
extern void begin_SUB();
extern void end_SUB();


void set_case(unsigned short int keyword_case)
{
    #define T_KW_SETTINGS_MACRO( NAME , ... )   \
        kw(#NAME)->print_case = keyword_case;
    #include "../t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


void set_text_original(unsigned short int ind_original)
{
    #define T_KW_SETTINGS_MACRO( NAME , ... )           \
        kw(#NAME)->print_original_text = ind_original;
    #include "../t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


void init_all_settings()
{
    #define T_KW_SETTINGS_MACRO(NAME, nlb, tb, sb, nla, ta, sa, TEXT, \
        fb1, fb2, fb3, fa1, fa2, fa3)           \
    do {                                        \
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
        kw(#NAME)->funct_before[0] = fb1;             \
        kw(#NAME)->funct_before[1] = fb2;             \
        kw(#NAME)->funct_before[2] = fb3;             \
        kw(#NAME)->funct_after [0] = fa1;             \
        kw(#NAME)->funct_after [1] = fa2;             \
        kw(#NAME)->funct_after [2] = fa3;             \
    } while (0);
    #include "../t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}
