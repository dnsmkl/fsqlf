#ifndef print_keywords_h
#define print_keywords_h


#include "global_variables.h"
#include "create_conf_file.h"
#include "settings.h"
#include <stdio.h>




static void print_nlines(FILE * yyout, int count){
    int i;
    for(i=0; i < count; i++) fputs("\n", yyout);
}


static void print_tabs(FILE * yyout, int count){
    int i;
    for(i=0; i < count; i++) fputs(tab_string, yyout);
}


static void print_spaces(FILE * yyout, int count){
    int i;
    for(i=0; i < count; i++) fputs(" ", yyout);
}


static void print_struct_spacing_count(FILE * yyout, spacing_counts s){
    print_nlines(yyout, s.new_line);
    print_tabs(yyout, s.indent);
    print_spaces(yyout, s.space);
}


// All printing of spacing goes through here.
// Except when space is inside the multiword-keyword (e.g. "LEFT JOIN"),
// those will not be printed by this function.
// ('spacing' means new lines, tabs and spaces)
static void print_spacing(
    FILE * yyout
    , t_kw_settings current_settings
    , int global_indent_level
){
    // keep track of 'after' spacing from previous call
    static spacing_counts from_previous__scounts = {0,0,0};
    // keep track of previous 'is_word'
    static unsigned short int from_previous__isword = 0;

    spacing_counts spacing =
        calculate_spacing(
            from_previous__scounts
            , from_previous__isword
            , current_settings.before
            , current_settings.is_word
            , global_indent_level
        );

    print_struct_spacing_count(yyout, spacing);

    // Save settings for next function call - overwrite
    from_previous__scounts = current_settings.after;
    from_previous__isword = current_settings.is_word;
}


void kw_print(FILE * yyout, char * yytext, t_kw_settings s){
    int i=0;
    // Call keyword specific functions, before printing.
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_before[i] != NULL ; i++)
        s.funct_before[i]();

    // Print spacing.
    print_spacing(yyout, s, currindent); // print spacing before keyword

    // Print text:
    // .. first decide what text to use (original or default)
    char * text_nocase = s.print_original_text ? yytext : s.text;
    // .. then handle its case
    char * text = stocase(text_nocase, s.print_case);
    // .. then print the text.
    fprintf(yyout, "%s", text);

    // Call keyword specific functions. after printing.
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_after[i] != NULL ; i++)
        s.funct_after[i]();
}


void echo_print(FILE * yyout, char * txt){
    int length; // length of the input text string
    length = strlen(txt);

    // Adjustment for single line comments.
    // Necessary for keeping indentation and new lines right.
    int pos_last_char = length - 1; // position of last character

    // Printing of spacing is delegated to print_spacing(),
    // which requires as input t_kw_settings.
    t_kw_settings s = {{0,0,0},{0,0,0},0,0,0,0};
 
    // Delegate to print_spacing() printing of the new line.
    if(txt[pos_last_char] == '\n'){
        // Shorten the string - overwrite \n with \0 (end of string mark).
        txt[pos_last_char] = '\0';
        // Adjust setting used by print_spacing()
        s.after.new_line = 1;
    }

    // Word-vs-operator check.
    // Ensure that two adjacent words have spacing inbetween.
    s.is_word = !(length == 1 && !isalnum(txt[0]));

    // print spacing then text
    print_spacing(yyout, s, currindent);
    fputs(txt, yyout);
}


void handle_kw(FILE * yyout, char * yytext, t_kw_settings s){
    kw_print(yyout, yytext, s);
}


void handle_text(FILE * yyout, char * txt){
    echo_print(yyout, txt);
}


#endif

