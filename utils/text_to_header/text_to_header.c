#include <stdio.h>
#include <stdlib.h>


void print_usage_info(const char *progname)
{
    printf("Usage:\n");
    printf("    %s <input-file> <output-file> <variable-name>\n", progname);
}


void exit_error_invalid_arguments(int argc, const char *progname)
{
    printf("Error: Invalid number of arguments.\n"
        "    Number of arguments supplied: %d.\n"
        "    Number of arguments expected: 3.\n", argc-1);
    print_usage_info(progname);
    exit(EXIT_FAILURE);
}


void exit_error_input_file_opening(const char* fname, const char *progname)
{
    printf("Error: Failed to open input file '%s'.\n", fname);
    print_usage_info(progname);
    exit(EXIT_FAILURE);
}


void exit_error_output_file_opening(const char* fname, const char *progname)
{
    printf("Error: Failed to open output file '%s'.\n", fname);
    print_usage_info(progname);
    exit(EXIT_FAILURE);
}


int main(int argc, char **argv)
{
    if (argc != 4) exit_error_invalid_arguments(argc, argv[0]);
    FILE *input = fopen(argv[1], "r");
    if (!input) exit_error_input_file_opening(argv[1], argv[0]);
    FILE *output = fopen(argv[2], "w");
    if (!output) exit_error_output_file_opening(argv[2], argv[0]);

    fprintf(output, "#ifndef %s_H\n", argv[3]);
    fprintf(output, "#define %s_H\n\n\n", argv[3]);
    fprintf(output, "const char *%s =\n", argv[3]);

    int counter = 0;
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        ++counter;
        if (counter == 1) fprintf(output, "    \"");
        else if (counter % 10 == 1) fprintf(output, "\"\n    \"");
        fprintf(output, "\\x%02x", ch);
    }
    fprintf(output, "\";\n\n\n");
    fprintf(output, "const size_t %s_LEN = %d;\n\n\n", argv[3], counter);
    fprintf(output, "#endif // %s_H\n", argv[3]);


    fclose(input);
    fclose(output);
    return EXIT_SUCCESS;
}
