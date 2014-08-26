#ifndef CONF_FILE_READ_H
#define CONF_FILE_READ_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "settings.h"


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


#define CONFIG_FILE "formatting.conf"
#define VALUE_NUMBER (6)
#define BUFFER_SIZE (100)
int read_configs()
{
    FILE * config_file;
    char line[BUFFER_SIZE+1] , setting_name[BUFFER_SIZE+1];
    int setting_values[VALUE_NUMBER];
    char * chr_ptr1;
    int i;

    config_file=fopen(CONFIG_FILE,"r"); // open file in working directory
    #ifndef _WIN32
    if(config_file == NULL)
    {
        // in non-windows (unix/linux) also try folder in user-home directory
        #define PATH_STRING_MAX_SIZE (200)
        char full_path[PATH_STRING_MAX_SIZE+1];
        strncpy(full_path, getenv("HOME") , PATH_STRING_MAX_SIZE);
        strncat(full_path, "/.fsqlf/" CONFIG_FILE ,PATH_STRING_MAX_SIZE - strlen(full_path));
        config_file=fopen(full_path,"r");
    }
    #endif

    if(config_file == NULL)
    {
        return 1;
    }

    while( fgets( line, BUFFER_SIZE, config_file ) )
    {
        if(line[0]=='#') continue; // lines starting with '#' are commnets
        // find and mark with '\0' where first stace is (end c string)
        if( !(chr_ptr1=strchr(line,' ')) ) continue;
        chr_ptr1[0]='\0';

        // store into variables
        strncpy( setting_name, line, BUFFER_SIZE );
        for(i = 0; i < VALUE_NUMBER; i++){
            setting_values[i] = strtol( chr_ptr1+1, &chr_ptr1, 10 );
        }

        // debug        printf("\nsetting_name='%s'; v0='%d'; v1='%d'; v2='%d'; v3='%d'; v4='%d'; v5='%d';",setting_name, setting_values[0], setting_values[1], setting_values[2], setting_values[3], setting_values[4], setting_values[5]);
        setting_value(setting_name,setting_values);
    }

    fclose(config_file);
    return 0;
}


#endif
