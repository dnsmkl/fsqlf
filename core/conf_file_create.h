#ifndef create_conf_file_h
#define create_conf_file_h


// Create formatting configuration file with default content.
// Return values:
//  0 on success
//  1 on failure to open
//  2 on failure to close
int create_config_file(char* config_file_name)
{
    FILE * config_file;
    config_file = fopen(config_file_name, "w");

    if (!config_file) {
        fprintf(stderr, "Failed to create '%s' file!\n", config_file_name);
        return 1;
    }

    fputs("# This file contains formatting (spacing) settings wich can be used to override the default formatting style used by FSQLF\n", config_file);
    fputs("# Lines starting with '#' are comments.  Comments and empty lines are ignored\n", config_file);
    fputs("# If there are couple of lines with same setting_name, then only the last one has effect\n\n" , config_file);

    fputs("# space_after ----------------------------------------------+\n", config_file);
    fputs("# tab_after -----------------------------------------+      |\n", config_file);
    fputs("# new_line_after -----------------------------+      |      |\n", config_file);
    fputs("# space_before ------------------------+      |      |      |\n", config_file);
    fputs("# tab_before -------------------+      |      |      |      |\n", config_file);
    fputs("# new_line_before -------+      |      |      |      |      |\n", config_file);
    fputs("#                        |      |      |      |      |      |\n", config_file);
    fputs("# setting_name\n", config_file);

    // Define macro to print one line containing config of single keyword
    // and run it (via #include) for each keyword.
    #define QQ(name) #name
    #define T_KW_SETTINGS_MACRO(NAME, nlb, tb, sb, nla, ta, sa, ... )   \
        fprintf(config_file, "%-24s %s %6s %6s %6s %6s %6s\n",          \
            QQ(NAME), QQ(nlb), QQ(tb), QQ(sb), QQ(nla), QQ(ta), QQ(sa));
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
    #undef QQ

    fputs("\n\n", config_file);
    fputs("# Some explanations regarding names (shortenings) used above:\n", config_file);
    fputs("# - left_p = left paranthesis\n", config_file);
    fputs("# - right_p = right paranthesis\n", config_file);
    fputs("# - ins = insert\n", config_file);
    fputs("# - sub = subquery\n", config_file);
    fputs("# - grpby = group by\n", config_file);
    fputs("# - ordby = order by\n", config_file);
    fputs("# - kw = keyword (though for some reason it's used also for puntuantion character..)\n", config_file);
    fputs("\n\n", config_file);

    if (fclose(config_file) == 0) {
        return 0;
    } else {
        fprintf(stderr, "Failed to close '%s' file!\n", config_file_name);
        return 2;
    }
}



#endif
