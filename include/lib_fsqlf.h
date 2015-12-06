#ifndef LIB_FSQLF_H
#define LIB_FSQLF_H


#include <stdio.h> // FILE
#include "../utils/map/uthash.h" // UT_hash_handle, HASH_ADD_KEYPTR, HASH_FIND_STR


struct spacing
{
    int global_indent_change;
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
struct kw_conf *kw_add(const char *name);
// Retrieve struct kw_cont from global map.
struct kw_conf *kw(const char *name);
// Remove all kw_cont from global map and free the memory.
void kw_delete_all();


void set_case(unsigned short int keyword_case);
void set_text_original(unsigned short int ind_original);
void init_all_settings(struct kw_conf * (*kw)(const char *));




// Create formatting configuration file with default content.
// Return values:
//  0 on success
//  1 on failure to open
//  2 on failure to close
int create_conf_file(char *config_file_name);


#define READ_SUCCESSFULL (0)
#define READ_FAILED (1)
// Read specified config file
int read_conf_file(const char *file_pathname,
                    struct kw_conf * (*kw)(const char *));


// Read configuration file from default conf file
// This would be "formatting.conf" in working idrectory
// If that does not exists, then on non-windows try "~/fslqf/formatting.conf"
int read_default_conf_file(struct kw_conf * (*kw)(const char *));


void fsqlf_set_file_in(FILE *in);
void fsqlf_set_file_out(FILE *out);


#endif
