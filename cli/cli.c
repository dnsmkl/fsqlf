#include <ctype.h> // isdigit
#include <stdlib.h> // exit
#include <stdio.h> // fprintf, stderr
#include <lib_fsqlf.h>
#include "../utils/string/read_int.h"   // FSQLF_read_int
#include "cli.h"




#define FAIL_WITH_ERROR( CODE , MESSAGE , ... )         \
do {                                                    \
    fprintf(stderr, "\n" MESSAGE "\n", __VA_ARGS__ );   \
    exit( CODE );                                       \
} while (0)


#define PRINT_OPTION_INFO( OPTION_TEXT , OPTION_DESCRIPTION ) \
    fprintf(stderr, "    " OPTION_TEXT "\n        " OPTION_DESCRIPTION "\n")


#define ARGV_MATCH(INDEX, TEXT) (strcmp(argv[INDEX], TEXT) == 0)


static void usage_info(int argc, char **argv)
{
    fprintf(stderr, "usage:\n" );
    PRINT_OPTION_INFO( "fsqlf [<input_file>] [<output_file>] [options]",
        "Read from <input_file> and write formatted output to <output_file> (use std I/O if missing)\n"
        "        If there are overlaping options set, then the last one (overlapping setting) wins.\n"
        "        e.g. If config file is set 2 times, then from 1st file use only configs that don't exist in the 2nd file.");
    PRINT_OPTION_INFO( "fsqlf --create-config-file <conf_file>", "Create config file. (file is overwritten if already exists)");
    fprintf(stderr, "options:\n");
    PRINT_OPTION_INFO( "-i <input_file>" , "Use <input_file> as input");
    PRINT_OPTION_INFO( "-o <output_file>" , "Use  <output_file> as output");
    PRINT_OPTION_INFO( "--config-file <config_file>"                , "Read configuration from <config_file>");
    PRINT_OPTION_INFO( "--select-comma-newline (after|before|none)" , "New lines for each item in SELECT clause");
    PRINT_OPTION_INFO( "--select-newline-after <digit>"             , "Put <digit> new lines right after SELECT keyword");
    PRINT_OPTION_INFO( "--newline-or-before <digit>"                , "Put <digit> new lines before OR keyword");
    PRINT_OPTION_INFO( "--newline-or-after <digit>"                 , "Put <digit> new lines before OR keyword");
    PRINT_OPTION_INFO( "--newline-and-before <digit>"               , "Put <digit> new lines before AND keyword");
    PRINT_OPTION_INFO( "--newline-and-after <digit>"                , "Put <digit> new lines before AND keyword");
    PRINT_OPTION_INFO( "--newline-major-sections <digit>"           , "Put <digit> new lines before major sections (FROM, JOIN, WHERE)");
    PRINT_OPTION_INFO( "--keyword-case (upper|lower|initcap|none)"  , "Convert all keywords to UPPER, lower, or Initcap case, or not to convert case at all");
    PRINT_OPTION_INFO( "--keyword-text (original|default)"          , "Use original or programs default text for the keyword, when there are several alternatives");
    PRINT_OPTION_INFO( "--help, -h"                                 , "Show this help.");
}


// Get argument and convert it to integer
static int get_int_arg(int i, int argc, char **argv)
{
    int r;
    if (!FSQLF_read_int(argv[i], 1000, &r)) {
        FAIL_WITH_ERROR(1, "Missing or invalid value for option : %s", argv[i-1]);
    }
    return r;
}


void read_cli_options(struct fsqlf_kw_conf *kwall, int argc, char **argv,
    FILE **fin, FILE **fout)
{
    int i;
    if (argc == 1) return; // use stdin and stdout

    if (argc == 2 && strcmp(argv[1], "--create-config-file") == 0) {
        FAIL_WITH_ERROR(1, "Missing value for option : %s", argv[1]);
    }
    if (argc == 3 && strcmp(argv[1], "--create-config-file") == 0) {
        if (fsqlf_kwmap_conffile_create(argv[2]) != FSQLF_OK) {
            fprintf(stderr, "Problem occurred during creation of config file '%s'.\n", argv[2]);
            exit(1);
        } else {
            fprintf(stderr, "Configuration was written to file '%s'.\n", argv[2]);
            exit(0);
        }
    }

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if ((*fin) == stdin) {
                //try to openinig INPUT file
                (*fin) = fopen(argv[1], "r");
                if (!(*fin)) {
                    FAIL_WITH_ERROR(1, "Error opening input file: %s", argv[i]);
                }
            }
            else if ((*fout) == stdout) {   //try to openinig OUTPUT file (only if INPUT file is set)
                (*fout) = fopen(argv[2], "w+");
                if (!(*fout)) FAIL_WITH_ERROR(1, "Error opening output file: %s", argv[i]);
            }
        } else if (ARGV_MATCH(i, "-i")) {
            if (++i >= argc) FAIL_WITH_ERROR(1, "Missing value for option : %s", argv[i-1]);
            (*fin) = fopen(argv[i], "r");
            if (!(*fin)) FAIL_WITH_ERROR(1, "Error opening input file: %s", argv[i]);
        } else if (ARGV_MATCH(i, "-o")) {
            if (++i >= argc) FAIL_WITH_ERROR(1, "Missing value for option : %s", argv[i-1]);
            (*fout) = fopen(argv[i], "w+");
            if (!(*fout)) FAIL_WITH_ERROR(1, "Error opening output file: %s", argv[i]);
        } else if (ARGV_MATCH(i, "--config-file")) {
            if (++i >= argc) FAIL_WITH_ERROR(1, "Missing value for option : %s", argv[i-1]);
            if (fsqlf_kwmap_conffile_read(kwall, argv[i]) == FSQLF_FAIL) {
                FAIL_WITH_ERROR(1, "Error reading configuration file: %s", argv[i]);
            }
        } else if (ARGV_MATCH(i, "--select-comma-newline")) {
            if (++i >= argc) FAIL_WITH_ERROR(1, "Missing value for option : %s", argv[i-1]);
            if (strcmp(argv[i], "after") == 0) {
                fsqlf_kw_get(kwall, "kw_comma")->before.new_line = 0;
                fsqlf_kw_get(kwall, "kw_comma")->after.new_line  = 1;
            } else if (strcmp(argv[i], "before") == 0) {
                fsqlf_kw_get(kwall, "kw_comma")->before.new_line = 1;
                fsqlf_kw_get(kwall, "kw_comma")->after.new_line  = 0;
            } else if (strcmp(argv[i], "none") == 0) {
                fsqlf_kw_get(kwall, "kw_comma")->before.new_line = 0;
                fsqlf_kw_get(kwall, "kw_comma")->after.new_line  = 0;
            }
        } else if (ARGV_MATCH(i, "--keyword-case")) {
            if (++i >= argc) FAIL_WITH_ERROR(1, "Missing value for option : %s", argv[i-1]);
            if (strcmp(argv[i], "none") == 0) {
                fsqlf_kwmap_set_case(kwall, FSQLF_KWCASE_ORIGINAL);
            } else if (strcmp(argv[i], "upper") == 0) {
                fsqlf_kwmap_set_case(kwall, FSQLF_KWCASE_UPPER);
            } else if (strcmp(argv[i], "lower") == 0) {
                fsqlf_kwmap_set_case(kwall, FSQLF_KWCASE_LOWER);
            } else if (strcmp(argv[i], "initcap") == 0) {
                fsqlf_kwmap_set_case(kwall, FSQLF_KWCASE_INITCAP);
            }
        } else if (ARGV_MATCH(i, "--keyword-text")) {
            if (++i >= argc) FAIL_WITH_ERROR(1, "Missing value for option : %s", argv[i-1]);
            if (strcmp(argv[i], "original") == 0) {
                fsqlf_kwmap_set_spelling(kwall, FSQLF_KWSPELLING_USE_ORIGINAL);
            } else if (strcmp(argv[i], "default") == 0) {
                fsqlf_kwmap_set_spelling(kwall, FSQLF_KWSPELLING_USE_HARDCODED_DEFAULT);
            }
        } else if (ARGV_MATCH(i, "--select-newline-after")) {
            fsqlf_kw_get(kwall, "kw_select")->after.new_line = get_int_arg(++i, argc, argv);
        } else if (ARGV_MATCH(i, "--newline-or-before")) {
            fsqlf_kw_get(kwall, "kw_or")->before.new_line = get_int_arg(++i, argc, argv);
        } else if (ARGV_MATCH(i, "--newline-or-after")) {
            fsqlf_kw_get(kwall, "kw_or")->after.new_line = get_int_arg(++i, argc, argv);
        } else if (ARGV_MATCH(i, "--newline-and-before")) {
            fsqlf_kw_get(kwall, "kw_and")->before.new_line = get_int_arg(++i, argc, argv);
        } else if (ARGV_MATCH(i, "--newline-and-after")) {
            fsqlf_kw_get(kwall, "kw_and")->after.new_line = get_int_arg(++i, argc, argv);
        } else if (ARGV_MATCH(i, "--newline-major-sections")) {
            int new_line_count = get_int_arg(++i, argc, argv);
            fsqlf_kwmap_set_major_clause_nl(kwall, new_line_count);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            usage_info(argc, argv);
            exit(0);
        } else FAIL_WITH_ERROR(1, "Option `%s' is not recognised or used incorrectly.\nTry `%s --help' for more information\n", argv[i], argv[0]);
    }
}
