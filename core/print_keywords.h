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
    int space_cnt=0, nl_cnt=0, length;

    t_kw_settings s;
    s.before.new_line=s.before.indent=s.before.space=s.after.new_line=s.after.indent=s.after.space=0;

    length = strlen(txt);

    // Prepare text for print (i is used with value set by last loop)
    s.text = (char*) malloc((length+1)*sizeof(char));
    if(!s.text) exit(1);

    strncpy(s.text, txt, length+1);


    s.is_word = ! (length == 1 && !isalnum(s.text[0]));

    // Spacing
    s.after.new_line = nl_cnt;
    s.after.space = space_cnt;
    print_spacing(yyout, s, currindent);

    fprintf(yyout,"%s",s.text);

    free(s.text);
}




#endif

