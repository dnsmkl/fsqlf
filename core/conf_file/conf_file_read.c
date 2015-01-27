#include <stdio.h>      // fopen, fclose
#include <stdlib.h>     // strtol
#include <string.h>     // strcmp, strchr, strncat, strncpy
#include <assert.h>     // assert
#include <sys/stat.h>   // stat

#include "conf_file_read.h"
#include "../kw/kw.h"   // All kw settings as global variables.


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
        k->before.new_line  = setting_values[0];
        k->before.indent    = setting_values[1];
        k->before.space     = setting_values[2];
        k->after.new_line   = setting_values[3];
        k->after.indent     = setting_values[4];
        k->after.space      = setting_values[5];
    }
}


#define READ_SUCCESSFULL (0)
#define READ_FAILED (1)
// Read specified config file
int read_conf_file(const char *file_pathname)
{
    FILE *config_file = fopen(file_pathname, "r");
    if (!config_file) {
        return READ_FAILED;
    }

    const int BUFFER_SIZE = 100;
    char line[BUFFER_SIZE+1], setting_name[BUFFER_SIZE+1];

    while (fgets(line, BUFFER_SIZE, config_file)) {
        // Lines starting with '#' are comments.
        if (line[0] == '#') continue;

        // Read kw name. Eat all characters until first space.
        // TODO: Improve handling of name not fitting into the buffer.
        //     At the moment first BUFFER_SIZE chars will be ingored
        //     and the rest would be treated as kw name.
        char *space_ptr = strchr(line, ' ');
        if (!space_ptr) continue;
        space_ptr[0] = '\0';
        strncpy(setting_name, line, BUFFER_SIZE);

        // Read numeric values.
        char *search_start = space_ptr + 1; // Skip \0 char.
        const int VALUE_COUNT = 6;
        int setting_values[VALUE_COUNT];
        for (int i = 0; i < VALUE_COUNT; i++) {
            setting_values[i] = strtol(search_start, &search_start, 10);
            // TODO: check if strtol conversion failed.
            // It returns 0L, which means we have to check string manualy.
            // Also check for case when not all settings fit into the buffer.
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
int read_default_conf_file()
{
    // First try file in working directory
    if (file_exists(FSQLF_CONFIG_FILE_NAME)) {
        return read_conf_file(FSQLF_CONFIG_FILE_NAME);
    }
    #ifndef _WIN32
        // in non-windows (unix/linux) also try folder in user-home directory
        // TODO: increase length, check for overflow.
        const size_t MAX_LEN = 200;
        char full_path[MAX_LEN + 1];
        strncpy(full_path, getenv("HOME"), MAX_LEN);
        strncat(full_path, "/.fsqlf/" FSQLF_CONFIG_FILE_NAME, MAX_LEN - strlen(full_path));
        return read_conf_file(full_path);
    #endif
}
