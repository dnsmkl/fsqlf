#include <stdio.h>      // fopen, fclose
#include <string.h>     // strcmp, strchr, strncat, strncpy, strlen
#include <assert.h>     // assert
#include <sys/stat.h>   // stat
#include <lib_fsqlf.h>
#include "../../utils/string/read_int.h"


static int file_exists(const char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}


static void set_from_array(struct fsqlf_kw_conf *kwall, const char *name, const int *values)
{
    struct fsqlf_kw_conf *k;
    k = fsqlf_kw_get(kwall, name);
    if (k != NULL) {
        k->before.global_indent_change  = values[0];
        k->before.new_line  = values[1];
        k->before.indent    = values[2];
        k->before.space     = values[3];
        k->after.global_indent_change   = values[4];
        k->after.new_line   = values[5];
        k->after.indent     = values[6];
        k->after.space      = values[7];
    }
}


// Read specified config file.
enum fsqlf_status fsqlf_kwmap_conffile_read(struct fsqlf_kw_conf *kwall, const char *file)
{
    FILE *config_file = fopen(file, "r");
    if (!config_file) {
        return FSQLF_FAIL;
    }

    char line[FSQLF_CONFFILE_LINELENGTH+1];
    char setting_name[FSQLF_CONFFILE_LINELENGTH+1];

    while (fgets(line, FSQLF_CONFFILE_LINELENGTH, config_file)) {
        // Lines starting with '#' are comments: skip them.
        // If line doesn't fit into buffer, it is invalid: skip it.
        if (line[0] == '#' || !strchr(line, '\n')) {
            do {
                // Skip chunks until new line is found
                if (!fgets(line, FSQLF_CONFFILE_LINELENGTH, config_file)) break;
            } while (!strchr(line, '\n'));
            // Skip that also, as it is continuation
            // of same line that we want to skip
            continue;
        }

        size_t llen = strlen(line);

        // Read setting name.
        char *space_ptr = strchr(line, ' ');
        if (!space_ptr) continue;
        space_ptr[0] = '\0';
        strncpy(setting_name, line, FSQLF_CONFFILE_LINELENGTH);

        // Read numeric setting values.
        // nl before, tab before, space before, nl after, tab after, space after
        char *pos = space_ptr + 1; // Skip \0 char.
        llen -= pos - line;
        const int VALUE_COUNT = 8;
        int setting_values[VALUE_COUNT];
        size_t cnt = FSQLF_read_int_array(pos, llen, VALUE_COUNT, setting_values);
        if (cnt == 0) {
            continue;
        }
        set_from_array(kwall, setting_name, setting_values);
    }

    fclose(config_file);
    return FSQLF_OK;
}


static char * get_path_to_user_folder_conf()
{
    // Get path to conf file in user (home) directory.

    // ..get all the ingredients.
    #ifndef _WIN32
    const char *home_dir = getenv("HOME");
    #else
    const char *home_dir = getenv("USERPROFILE");
    #endif
    if (home_dir == NULL) return NULL;
    const char *fsqlf_sub = "/.fsqlf/";
    const char *conf_file = FSQLF_CONFFILE_NAME;

    // ..get complete path to conf file.
    size_t full_len = strlen(home_dir) + strlen(fsqlf_sub)
            + strlen(conf_file) + 1;
    char * full_path = malloc(full_len);
    if (full_path == NULL) return NULL;
    strncpy(full_path, home_dir, full_len);
    strncat(full_path, fsqlf_sub, full_len - strlen(full_path));
    strncat(full_path, conf_file, full_len - strlen(full_path));
    return full_path;
}


// Read configuration file from default conf file.
// (working directory and "fsqlf/" under user dir)
enum fsqlf_status fsqlf_kwmap_conffile_read_default(struct fsqlf_kw_conf *kwall)
{
    // First try file in working directory.
    if (file_exists(FSQLF_CONFFILE_NAME)) {
        return fsqlf_kwmap_conffile_read(kwall, FSQLF_CONFFILE_NAME);
    }

    char *full_path = get_path_to_user_folder_conf();
    if (full_path == NULL) return FSQLF_FAIL;

    // Read the file.
    enum fsqlf_status ret_code = fsqlf_kwmap_conffile_read(kwall, full_path);

    // Cleanup.
    free(full_path);

    return ret_code;
}
