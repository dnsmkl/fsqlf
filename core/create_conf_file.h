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
    fputs("# This file contains formatting (spacing) settings wich can be used to override the default formatting style used by FSQLF\n", config_file);
    fputs("# Lines starting with '#' are comments.  Comments and empty lines are ignored\n", config_file);
    fputs("# If there are couple of lines with same setting_name, then only the last one has effect\n\n" , config_file);

    fputs("# space_after -----------------------------------------+\n", config_file);
    fputs("# tab_after ------------------------------------+      |\n", config_file);
    fputs("# new_line_after ------------------------+      |      |\n", config_file);
    fputs("# space_before -------------------+      |      |      |\n", config_file);
    fputs("# tab_before --------------+      |      |      |      |\n", config_file);
    fputs("# new_line_before --+      |      |      |      |      |\n", config_file);
    fputs("#                   |      |      |      |      |      |\n", config_file);
    fputs("# setting_name\n", config_file);

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


    fputs("\n\n", config_file);


    if (fclose(config_file) == 0) // Write and close config file
        return 0;
    else {
        fprintf(stderr, "Failed to close '%s' file!\n", config_file_name);
        return 2;
    }
}



#endif
