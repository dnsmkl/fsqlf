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


static void print_spacing(FILE * yyout, t_kw_settings current_settings, int global_indent_level){
/* Prints all spacing of the program
 * Except when space is inside the multiword-keyword (e.g. "LEFT JOIN"), those will not be printed by this function
 * 'spacing' means new lines, tabs and spaces
*/

    static spacing_counts from_previous__scounts = {0,0,0}; // keep track of 'after' spacing from previous call
    static unsigned short int from_previous__isword = 0;    // keep track of previous 'is_word'

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
    // call keyword specific functions. Before fprintf
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_before[i] != NULL ; i++)
        s.funct_before[i]();
    print_spacing(yyout, s, currindent); // print spacing before keyword
    fprintf(yyout,"%s",stocase( s.print_original_text ? yytext : s.text , s.print_case)); // 1st deside what text to use (original or degault), then handle its case
    // call keyword specific functions. After fprintf
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_after[i] != NULL ; i++)
        s.funct_after[i]();
}


void echo_print(FILE * yyout, char * txt){
    int length; // length of the input text string
    int pos_last_char; // position of last character
    t_kw_settings s; // printing of spacing is delegated to print_spacing(), which needs t_kw_settings as input
    
    s.before.new_line=s.before.indent=s.before.space=s.after.new_line=s.after.indent=s.after.space=0;

    length = strlen(txt);

    // adjustment for single line comments - necessary for keeping indentation and new lines right
    pos_last_char = length - 1;
    if(txt[pos_last_char] == '\n'){
        txt[pos_last_char] = '\0'; // shorten the string - overwrite \n (new line) with \0 (end of string mark)
        s.after.new_line = 1; // delegate to print_spacing() printing of the new line
    }

    // word-vs-operator check - ensures that two adjacent words have spacing inbetween
    s.is_word = !(length == 1 && !isalnum(txt[0]));

    // print spacing then text 
    print_spacing(yyout, s, currindent);
    fputs(txt, yyout);
}




#endif

