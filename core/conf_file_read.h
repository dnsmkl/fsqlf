#ifndef CONF_FILE_READ_H
#define CONF_FILE_READ_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>   // For stat().

#include "settings.h"


int file_exists(char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}


void setting_value(char * setting_name, int * setting_values)
{
    #define T_KW_SETTINGS_MACRO( NAME, ... )    \
    if( strcmp(#NAME,setting_name) == 0 ){      \
        NAME.before.new_line    = setting_values[0];  \
        NAME.before.indent      = setting_values[1];  \
        NAME.before.space       = setting_values[2];  \
        NAME.after.new_line     = setting_values[3];  \
        NAME.after.indent       = setting_values[4];  \
        NAME.after.space        = setting_values[5];  \
    }
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


#define VALUE_NUMBER (6)
#define BUFFER_SIZE (100)
// Read specified config file
int read_conf_file(const char* file_pathname)
{
    FILE * config_file;
    char line[BUFFER_SIZE+1] , setting_name[BUFFER_SIZE+1];
    int setting_values[VALUE_NUMBER];
    char * chr_ptr1;
    int i;

    if(!(config_file=fopen(file_pathname,"r")))
    {
        return 1;
    }

    while( fgets( line, BUFFER_SIZE, config_file ) )
    {
        // Lines starting with '#' are comments.
        if(line[0]=='#') continue;

        // Read setting name. It starts at line start and ends with space.
        if( !(chr_ptr1=strchr(line,' ')) ) continue;
        chr_ptr1[0]='\0';
        strncpy( setting_name, line, BUFFER_SIZE );

        // Read setting values.
        for(i = 0; i < VALUE_NUMBER; i++){
            setting_values[i] = strtol( chr_ptr1+1, &chr_ptr1, 10 );
        }

        // Assign read values to global variables.
        setting_value(setting_name,setting_values);
    }

    fclose(config_file);
    return 0;
}



#define CONFIG_FILE "formatting.conf"
// Read configuration file from default conf file
// This would be "formatting.conf" in working idrectory
// If that does not exists, then on non-windows try "~/fslqf/formatting.conf"
// TODO: rename to read_default_conf_files
int read_configs()
{
    if(file_exists(CONFIG_FILE)) // check file in working directory
    {
        return read_conf_file(CONFIG_FILE);
    }
    #ifndef _WIN32
        // in non-windows (unix/linux) also try folder in user-home directory
        #define PATH_STRING_MAX_SIZE (200)
        char full_path[PATH_STRING_MAX_SIZE+1];
        strncpy(full_path, getenv("HOME") , PATH_STRING_MAX_SIZE);
        strncat(full_path, "/.fsqlf/" CONFIG_FILE ,PATH_STRING_MAX_SIZE - strlen(full_path));
        return read_conf_file(full_path);
    #endif
}


#endif
