#include <stdio.h>  // FILE, fprintf, fputs, fclose
#include <lib_fsqlf.h>
#include "conf_file_constants.h"


// Create formatting configuration file with default content.
// Return values:
//  0 on success
//  1 on failure to open
//  2 on failure to close
int create_conf_file(char *config_file_name)
{
    FILE *config_file;
    config_file = fopen(config_file_name, "w");

    if (!config_file) {
        fprintf(stderr, "Failed to create '%s' file!\n", config_file_name);
        return 1;
    }

    fputs("# This file contains formatting (spacing) settings,\n", config_file);
    fputs("# which can be used to override the default formatting style of FSQLF.\n", config_file);
    fputs("#\n", config_file);
    fputs("# Lines that are ignored (in this file):\n", config_file);
    fputs("# - Lines starting with '#' are treated as comments, thus ignored.\n", config_file);
    fputs("# - Last line is ignored (blank line or comment at the end of file is needed).\n", config_file);
    fputs("# - Empty lines are ignored (they fall into invalid line category).\n", config_file);
    fputs("# - Invalid config lines are ignored (see below for details).\n", config_file);
    fputs("#\n", config_file);
    fputs("# Valid config lines satisfy following criteria:\n", config_file);
    fputs("# - Contains `setting name` followed by 6 integer values.\n", config_file);
    fputs("# - Line begins with `setting name` (no leading spacing).\n", config_file);
    fputs("# - Values are separated by spaces (not tabs, not commas, not anything else).\n", config_file);
    fputs("# - `Setting name` should match one of predefined FSQLF setting names.\n", config_file);
    fprintf(config_file, "# - Line limit is %d characters (longer lines are ignored).\n", FSQLF_CONFFILE_LINELENGTH);
    fputs("# (if some criteria is violated, then line is invalid - thus ignored).\n", config_file);
    fputs("#\n", config_file);
    fputs("# If there are couple of valid lines with same setting_name,\n" , config_file);
    fputs("# then only the last one has effect.\n" , config_file);
    fputs("\n", config_file);
    fputs("\n", config_file);

    fputs("# space_after ------------------------------------------------------------+\n",config_file);
    fputs("# tab_after -------------------------------------------------------+      |\n",config_file);
    fputs("# new_line_after -------------------------------------------+      |      |\n",config_file);
    fputs("# global_indent_change_after-------------------------+      |      |      |\n",config_file);
    fputs("#                                                    |      |      |      |\n",config_file);
    fputs("# space_before -------------------------------+      |      |      |      |\n",config_file);
    fputs("# tab_before --------------------------+      |      |      |      |      |\n",config_file);
    fputs("# new_line_before --------------+      |      |      |      |      |      |\n",config_file);
    fputs("# global_indent_                |      |      |      |      |      |      |\n",config_file);
    fputs("# _change_before --------+      |      |      |      |      |      |      |\n",config_file);
    fputs("#                        |      |      |      |      |      |      |      |\n",config_file);
    fputs("# setting_name           |      |      |      |      |      |      |      |\n",config_file);

    // Define macro to print one line containing config of single keyword
    // and run it (via #include) for each keyword.
    #define XMACRO(NAME, gib, nlb, tb, sb, gia, nla, ta, sa, ... )   \
        fprintf(config_file, "%-24s %s %6s %6s %6s %6s %6s %6s %6s\n",          \
            #NAME, #gib, #nlb, #tb, #sb, #gia, #nla, #ta, #sa);
    #include "../kw/kw_defaults.def"
    #undef XMACRO

    fputs("\n\n", config_file);
    fputs("# Some explanations regarding names (shortenings) used above:\n", config_file);
    fputs("# - left_p = left parenthesis\n", config_file);
    fputs("# - right_p = right parenthesis\n", config_file);
    fputs("# - ins = insert\n", config_file);
    fputs("# - sub = subquery\n", config_file);
    fputs("# - grpby = group by\n", config_file);
    fputs("# - ordby = order by\n", config_file);
    fputs("# - kw = keyword (though for some reason it's used also for punctuation character..)\n", config_file);
    fputs("\n\n", config_file);

    if (fclose(config_file) == 0) {
        return 0;
    } else {
        fprintf(stderr, "Failed to close '%s' file!\n", config_file_name);
        return 2;
    }
}
