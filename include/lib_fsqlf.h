#ifndef LIB_FSQLF_H
#define LIB_FSQLF_H


#include <stdio.h> // FILE
#include "../utils/map/uthash.h" // UT_hash_handle, HASH_ADD_KEYPTR, HASH_FIND_STR


// Return value (in case function returns status code).
// At the moment only OK and FAIL,
// but later might add some more granular codes.
enum fsqlf_status
{
    FSQLF_OK = 0,
    FSQLF_FAIL = 1
};


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
struct fsqlf_kw_conf
{
    struct spacing before;
    struct spacing after;

    enum fsqlf_kwtext print_original_text;
    enum fsqlf_kwcase print_case;
    unsigned short int is_word; // two adjacent words MUST be separated by some spacing
    char *text;

    const char *name;
    UT_hash_handle hh;  // makes this structure hashable
};


// Create new struct fsqlf_kw_conf and add it to global map.
struct fsqlf_kw_conf *FSQLF_kw_create(const char *name);

// Retrieve struct fsqlf_kw_conf from global map.
struct fsqlf_kw_conf *fsqlf_kw_get(const char *name);

// Remove all kw_conf from global map and free the memory.
void fsqlf_kw_delete_all();


void fsqlf_set_all_kwcase(enum fsqlf_kwcase keyword_case);

// Set used variation of keyword text. (e.g. "left outer join" vs "left join")
void fsqlf_set_all_kwvariant(enum fsqlf_kwtext kw_text_to_use);

// Init all keyword settings to defaults.
void fsqlf_init_all_kw(struct fsqlf_kw_conf * (*fsqlf_kw_get)(const char *));




// Create formatting configuration file with default content.
enum fsqlf_status fsqlf_create_conf_file(char *config_file_name);


// Read specified config file.
enum fsqlf_status fsqlf_read_conf_file(const char *file_pathname,
                    struct fsqlf_kw_conf * (*fsqlf_kw_get)(const char *));


// Read configuration file from default conf file.
// This would be "formatting.conf" in working directory.
// If that does not exists, then on non-windows try "~/fslqf/formatting.conf".
enum fsqlf_status fsqlf_read_default_conf_file(struct fsqlf_kw_conf * (*fsqlf_kw_get)(const char *));


void fsqlf_set_file_in(FILE *in);
void fsqlf_set_file_out(FILE *out);


#endif
