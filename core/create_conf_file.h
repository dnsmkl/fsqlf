#ifndef create_conf_file_h
#define create_conf_file_h

int create_config_file( char* config_file_name ) {
    FILE * config_file;
    config_file = fopen(config_file_name, "w");

    if ( !(config_file) ){ // Create config file
        fprintf(stderr, "Failed to create '%s' file!\n", config_file_name);
        return 1;
    }
        // Write default configuration to "formatting.conf" file:
    fprintf(config_file, "# This file contains formatting (spacing) settings wich can be used to override the default formatting style used by FSQLF\n" );
    fprintf(config_file, "# Lines starting with '#' are comments\n" );
    fprintf(config_file, "# Each formatting (spacing) setting line contains:\n\n" );
    fprintf(config_file, "# setting_name  new_line_before tab_before space_before new_line_after tab_after space_after\n" );

    #define     QUOTE(name)     #name
    #define T_KW_SETTINGS_MACRO( NAME,nlb,tb,sb,nla,ta,sa,TEXT , ... ) \
        fprintf(config_file, "%-20s", QUOTE(NAME) ); \
        fprintf(config_file, "%s", QUOTE(nlb) ); \
        fprintf(config_file, "%7s",QUOTE(tb) ); \
        fprintf(config_file, "%7s",QUOTE(sb) ); \
        fprintf(config_file, "%7s",QUOTE(nla) ); \
        fprintf(config_file, "%7s",QUOTE(ta) ); \
        fprintf(config_file, "%7s",QUOTE(sa) ); \
        fprintf(config_file, "\n") ;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO


    fprintf(config_file, "\n\n");
    fprintf(config_file, "# If there are couple of lines with same setting_name, then only the last one has effect\n" );
    fprintf(config_file, "# Empty lines are ignored\n" );

    if (fclose(config_file) == 0) // Write and close config file
        return 0;
    else {
        fprintf(stderr, "Failed to close '%s' file!\n", config_file_name);
        return 2;
    }
}



#endif
