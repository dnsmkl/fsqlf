#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"
#include "global_variables.h"
#include "create_conf_file.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


// tab_string is needed to be able switch between spaces "    " and tabs '\t'
char * tab_string = "    ";



inline int max(int a, int b){
    return a > b ? a : b;
}

#define KW_FUNCT_ARRAY_SIZE (3)
#define CONFIG_FILE "formatting.conf"

// struture to store text keyword text  space,tab,newline, function to execute  before/after printig the keyword
typedef struct{
    int new_line;
    int indent;
    int space;
} spacing_counts;


typedef struct{
    spacing_counts before;
    spacing_counts after;

    unsigned short int print_original_text;
    unsigned short int print_case;
    unsigned short int is_word; // two adjacent words MUST be separated by some spacing
    char * text;

    void (*funct_before[KW_FUNCT_ARRAY_SIZE])();
    void (*funct_after [KW_FUNCT_ARRAY_SIZE])();
} t_kw_settings;



void debug_kw_settings(t_kw_settings s){
    extern FILE * yyout;
    fprintf(yyout,"\nspace_before %d , tab_before %d , nl_before %d , space_after %d , tab_after %d , nl_after %d\n , text %s "
           ,s.before.space, s.before.indent, s.before.new_line, s.after.space, s.after.indent, s.after.new_line, s.text);
    //printf("after %X %X %X\n", s.funct_after[0],s.funct_after[1],s.funct_after[2]);//debug string
    //printf("before %X %X %X\n", s.funct_before[0],s.funct_before[1],s.funct_before[2]);//debug string
}



void debug_p();// TODO : make separate .c and .h files



int max_or_current(prev_count, curr_count, use_only_curr_ind){
    if(use_only_curr_ind){
        return curr_count;
    }
    else{
        return max(prev_count, curr_count);
    }
}


static spacing_counts calculate_spacing(
    spacing_counts afterspacing_of_prev
    ,unsigned short int isword_of_prev
    ,spacing_counts beforespacing_of_current
    ,unsigned short int isword_of_current
    ,int global_indent_level
){
    /* Combine previous and current words adjacent spacings.
        Combination is done by using MAX or just using current words settings.
        Decision is made so:
            There are 3 types of spacings and have ranks: space-1, indentation-2, new line-3.
            if current word's has active spacing setting (not 0),
            then all spacings of lower ranks, must be used from current word
        Rationale for such logic:
            if word has in its settings, new-line=1 and indent/spaces=0,
            then there is expectation of keyword being at the start of the line (not counting global indent level because of subselect)
    */
    spacing_counts r; // result to be built


    r.new_line = max(afterspacing_of_prev.new_line, beforespacing_of_current.new_line);

    r.indent = max_or_current(
            afterspacing_of_prev.indent
            , beforespacing_of_current.indent
            , beforespacing_of_current.new_line>1);
    r.indent += r.new_line ? global_indent_level : 0;

    r.space = max_or_current(
            afterspacing_of_prev.space
            , beforespacing_of_current.space
            , beforespacing_of_current.new_line || beforespacing_of_current.indent);


    // adjacent words MUST have some spacing
    if(!r.new_line && !r.indent && !r.space
        && isword_of_prev && isword_of_current
    ){
        r.space = 1;
    }


    return r;
}


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


#define MAX_KEYWORD_SIZE (50)
enum{CASE_none,CASE_lower,CASE_UPPER,CASE_Initcap};
char* stocase(char* s_text, unsigned short int s_case){
    static char formatted_result[MAX_KEYWORD_SIZE];
    int i;

    switch(s_case){
        case CASE_lower:
            for(i=0; i<strlen(s_text); i++) formatted_result[i] = tolower(s_text[i]);
            break;
        case CASE_UPPER:
            for(i=0; i<strlen(s_text); i++) formatted_result[i] = toupper(s_text[i]);
            break;
        case CASE_Initcap:
            formatted_result[0] = toupper(s_text[0]);
            for(i=1; i<strlen(s_text); i++) formatted_result[i] = tolower(s_text[i]);
            break;
        case CASE_none:
            return s_text;
    }
    formatted_result[strlen(s_text)]='\0';
    return formatted_result;
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


// initialize t_kw_settings variables
#define T_KW_SETTINGS_MACRO( NAME , ... ) \
    t_kw_settings NAME ;
#include "t_kw_settings_list.def"
#undef T_KW_SETTINGS_MACRO



void set_case(unsigned short int keyword_case){
    #define T_KW_SETTINGS_MACRO( NAME , ... ) \
        NAME.print_case = keyword_case;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


void set_text_original(unsigned short int ind_original){
    #define T_KW_SETTINGS_MACRO( NAME , ... ) \
        NAME.print_original_text = ind_original;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}

void init_all_settings(){
    #define T_KW_SETTINGS_MACRO( NAME,nlb,tb,sb,nla,ta,sa,TEXT , fb1,fb2,fb3,fa1,fa2,fa3) \
        NAME.before.new_line    = nlb;    \
        NAME.before.indent      = tb;     \
        NAME.before.space       = sb;     \
                                    \
        NAME.after.new_line     = nla;    \
        NAME.after.indent       = ta;     \
        NAME.after.space        = sa;     \
        NAME.print_original_text = 0; \
        NAME.print_case         = CASE_UPPER; \
        NAME.text               = TEXT;   \
        NAME.is_word            = 1; \
                                    \
        NAME.funct_before[0] = fb1; \
        NAME.funct_before[1] = fb2; \
        NAME.funct_before[2] = fb3; \
        NAME.funct_after [0] = fa1; \
        NAME.funct_after [1] = fa2; \
        NAME.funct_after [2] = fa3;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}




#define BUFFER_SIZE (100)
#define VALUE_NUMBER (6)

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







#include <stdlib.h>
#include <string.h>

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
        // in non-windows (probably unix/linux) also try folder in user-home directory
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
