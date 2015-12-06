#include <stdio.h>      // fopen, fclose
#include <string.h>     // strcmp, strchr, strncat, strncpy, strlen
#include <assert.h>     // assert
#include <sys/stat.h>   // stat
#include <lib_fsqlf.h>
#include "conf_file_constants.h"
#include "../../utils/string/read_int.h"


static int file_exists(const char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}


static void setting_value(const char *setting_name, const int *setting_values)
{
    struct kw_conf *k;
    k = kw(setting_name);
    if (k != NULL) {
        k->before.global_indent_change  = setting_values[0];
        k->before.new_line  = setting_values[1];
        k->before.indent    = setting_values[2];
        k->before.space     = setting_values[3];
        k->after.global_indent_change   = setting_values[4];
        k->after.new_line   = setting_values[5];
        k->after.indent     = setting_values[6];
        k->after.space      = setting_values[7];
    }
}


#define READ_SUCCESSFULL (0)
#define READ_FAILED (1)
// Read specified config file
int fsqlf_read_conf_file(const char *file_pathname,
                    struct kw_conf * (*kw)(const char *))
{
    FILE *config_file = fopen(file_pathname, "r");
    if (!config_file) {
        return READ_FAILED;
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
        size_t cnt = read_int_array(pos, llen, VALUE_COUNT, setting_values);
        if (cnt == 0) {
            continue;
        }
        setting_value(setting_name, setting_values);
    }

    fclose(config_file);
    return READ_SUCCESSFULL;
}



// Read configuration file from default conf file
// This would be "formatting.conf" in working idrectory
// If that does not exists, then on non-windows try "~/fslqf/formatting.conf"
// TODO: rename to read_default_conf_files
int fsqlf_read_default_conf_file(struct kw_conf * (*kw)(const char *))
{
    // First try file in working directory
    if (file_exists(FSQLF_CONFFILE_NAME)) {
        return fsqlf_read_conf_file(FSQLF_CONFFILE_NAME, kw);
    }

    // In non-windows (unix/linux) also try folder in user-home directory
    #ifndef _WIN32
        // Get all the ingredients
        const char *home_dir = getenv("HOME");
        if (home_dir == NULL) return READ_FAILED; // TODO: Log it.
        const char *fsqlf_sub = "/.fsqlf/";
        const char *conf_file = FSQLF_CONFFILE_NAME;

        // Mix the ingredients - get complete path till conf file.
        size_t full_len = strlen(home_dir) + strlen(fsqlf_sub)
                        + strlen(conf_file) + 1;
        char * full_path = malloc(full_len);
        if (full_path == NULL) return READ_FAILED; // TODO: Log it.
        strncpy(full_path, home_dir, full_len);
        strncat(full_path, fsqlf_sub, full_len - strlen(full_path));
        strncat(full_path, conf_file, full_len - strlen(full_path));

        // Read the file
        int ret_code = fsqlf_read_conf_file(full_path, kw);

        // Cleanup
        free(full_path);

        return ret_code;
    #endif
}
