#ifndef settings_h
#define settings_h


#include <string.h>
#include <ctype.h>


const char * tab_string = "    "; //indentation is done with 4 spaces


// struture to store text keyword text  space,tab,newline, function to execute  before/after printig the keyword
typedef struct
{
    int new_line;
    int indent;
    int space;
} spacing_counts;


#define KW_FUNCT_ARRAY_SIZE (3)
typedef struct
{
    spacing_counts before;
    spacing_counts after;

    unsigned short int print_original_text;
    unsigned short int print_case;
    unsigned short int is_word; // two adjacent words MUST be separated by some spacing
    char * text;

    void (*funct_before[KW_FUNCT_ARRAY_SIZE])();
    void (*funct_after [KW_FUNCT_ARRAY_SIZE])();
} t_kw_settings;


void debug_p(); // TODO : make separate .c and .h files


// Initialize t_kw_settings variables.
#define T_KW_SETTINGS_MACRO( NAME , ... ) \
    t_kw_settings NAME ;
#include "t_kw_settings_list.def"
#undef T_KW_SETTINGS_MACRO


void set_case(unsigned short int keyword_case)
{
    #define T_KW_SETTINGS_MACRO( NAME , ... ) \
        NAME.print_case = keyword_case;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


void set_text_original(unsigned short int ind_original)
{
    #define T_KW_SETTINGS_MACRO( NAME , ... ) \
        NAME.print_original_text = ind_original;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


enum {CASE_none, CASE_lower, CASE_UPPER, CASE_Initcap};


void init_all_settings()
{
    #define T_KW_SETTINGS_MACRO(NAME, nlb, tb, sb, nla, ta, sa, TEXT, \
        fb1, fb2, fb3, fa1, fa2, fa3)           \
    do {                                        \
        NAME.before.new_line    = nlb;          \
        NAME.before.indent      = tb;           \
        NAME.before.space       = sb;           \
                                                \
        NAME.after.new_line     = nla;          \
        NAME.after.indent       = ta;           \
        NAME.after.space        = sa;           \
        NAME.print_original_text = 0;           \
        NAME.print_case         = CASE_UPPER;   \
        NAME.text               = TEXT;         \
        NAME.is_word            = 1;            \
                                                \
        NAME.funct_before[0] = fb1;             \
        NAME.funct_before[1] = fb2;             \
        NAME.funct_before[2] = fb3;             \
        NAME.funct_after [0] = fa1;             \
        NAME.funct_after [1] = fa2;             \
        NAME.funct_after [2] = fa3;             \
    } while (0);
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


#endif
