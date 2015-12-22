#ifndef LIB_FSQLF_H
#define LIB_FSQLF_H


#include <stdio.h> // FILE
#include "../utils/map/uthash.h" // UT_hash_handle, HASH_ADD_KEYPTR, HASH_FIND_STR


enum fsqlf_kwcase
{
    // Use whatever was in the input.
    FSQLF_KWCASE_ORIGINAL = 0,

    // Example: "select".
    FSQLF_KWCASE_LOWER = 1,

    // Example: "SELECT".
    FSQLF_KWCASE_UPPER = 2,

    // Example: "Select".
    FSQLF_KWCASE_INITCAP = 3
};


// Keyword text variation to be used.
// (e.g. "left outer join" vs "left join")
enum fsqlf_kwtext
{
    // Use whatever was in the input.
    FSQLF_KWTEXT_USE_ORIGINAL = 0,

    // Convert some keywords to fsqlf defaults.
    // Defaults are hard-coded in "kw_defaults.def".
    // Current defaults prefer:
    // - full words (e.g. convert "sel" to "select")
    // - lower word count (e.g. convert "left outer join" to "left join")
    FSQLF_KWTEXT_USE_HARDCODED_DEFAULT = 1,
};


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

    enum fsqlf_kwtext print_original_text;
    enum fsqlf_kwcase print_case;
    unsigned short int is_word; // two adjacent words MUST be separated by some spacing
    char *text;

    void (*funct_before[KW_FUNCT_ARRAY_SIZE])();

    const char *name;
    UT_hash_handle hh;  // makes this structure hashable
};


// Create new struct kw_conf and add it to global map.
struct kw_conf *kw_add(const char *name);
// Retrieve struct kw_conf from global map.
struct kw_conf *kw(const char *name);
// Remove all kw_conf from global map and free the memory.
void kw_delete_all();


void fsqlf_set_all_kwcase(enum fsqlf_kwcase keyword_case);

// Set used variation of keyword text. (e.g. "left outer join" vs "left join")
void fsqlf_set_all_kwvariant(enum fsqlf_kwtext kw_text_to_use);

void init_all_settings(struct kw_conf * (*kw)(const char *));




// Create formatting configuration file with default content.
// Return values:
//  0 on success
//  1 on failure to open
//  2 on failure to close
int fsqlf_create_conf_file(char *config_file_name);


#define READ_SUCCESSFULL (0)
#define READ_FAILED (1)
// Read specified config file
int fsqlf_read_conf_file(const char *file_pathname,
                    struct kw_conf * (*kw)(const char *));


// Read configuration file from default conf file
// This would be "formatting.conf" in working idrectory
// If that does not exists, then on non-windows try "~/fslqf/formatting.conf"
int fsqlf_read_default_conf_file(struct kw_conf * (*kw)(const char *));


void fsqlf_set_file_in(FILE *in);
void fsqlf_set_file_out(FILE *out);


#endif
