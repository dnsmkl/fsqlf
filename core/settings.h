#ifndef settings_h
#define settings_h


#include <string.h>
#include <ctype.h>
#include <stdlib.h>  // malloc


#include "../utils/map/uthash.h"


const char * tab_string = "    "; //indentation is done with 4 spaces


// struture to store text keyword text  space,tab,newline, function to execute  before/after printig the keyword
typedef struct
{
    int new_line;
    int indent;
    int space;
} spacing_counts;


#define KW_FUNCT_ARRAY_SIZE (3)
typedef struct kw_conf
{
    spacing_counts before;
    spacing_counts after;

    unsigned short int print_original_text;
    unsigned short int print_case;
    unsigned short int is_word; // two adjacent words MUST be separated by some spacing
    char *text;

    void (*funct_before[KW_FUNCT_ARRAY_SIZE])();
    void (*funct_after [KW_FUNCT_ARRAY_SIZE])();

    const char *name;
    UT_hash_handle hh;  // makes this structure hashable
} t_kw_settings;



// Global map and its manipulation functions.
// map it self should not be accessed directly.
struct kw_conf *g_keyword_config = NULL;


struct kw_conf* kw_add(const char *name)
{
    struct kw_conf *tmp;
    tmp = (struct kw_conf*) malloc(sizeof(struct kw_conf));
    assert(tmp);
    tmp->name = name;
    HASH_ADD_KEYPTR(hh, g_keyword_config, tmp->name, strlen(tmp->name), tmp);
    return tmp;
}


struct kw_conf* kw(const char *name)
{
    struct kw_conf *match;
    HASH_FIND_STR(g_keyword_config, name, match);
    assert(match != NULL);
    return match;
}


void debug_p(); // TODO : make separate .c and .h files


void set_case(unsigned short int keyword_case)
{
    #define T_KW_SETTINGS_MACRO( NAME , ... )   \
        kw(#NAME)->print_case = keyword_case;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


void set_text_original(unsigned short int ind_original)
{
    #define T_KW_SETTINGS_MACRO( NAME , ... )           \
        kw(#NAME)->print_original_text = ind_original;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


enum {CASE_none, CASE_lower, CASE_UPPER, CASE_Initcap};


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
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


#endif
