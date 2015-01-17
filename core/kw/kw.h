#ifndef KW_H
#define KW_H


#include <stdlib.h>  // malloc
#include <assert.h>  // assert


#include "../../utils/map/uthash.h"


struct spacing
{
    int new_line;
    int indent;
    int space;
};


#define KW_FUNCT_ARRAY_SIZE (3)
struct kw_conf
{
    struct spacing before;
    struct spacing after;

    unsigned short int print_original_text;
    unsigned short int print_case;
    unsigned short int is_word; // two adjacent words MUST be separated by some spacing
    char *text;

    void (*funct_before[KW_FUNCT_ARRAY_SIZE])();
    void (*funct_after [KW_FUNCT_ARRAY_SIZE])();

    const char *name;
    UT_hash_handle hh;  // makes this structure hashable
};


enum
{
	CASE_none,
	CASE_lower,
	CASE_UPPER,
	CASE_Initcap
};


// Create new struct kw_cont and add it to global map.
struct kw_conf* kw_add(const char *name);
// Retrieve struct kw_cont from global map.
struct kw_conf* kw(const char *name);


#endif
