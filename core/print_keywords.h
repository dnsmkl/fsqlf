#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"
#include "global_variables.h"
#include "create_conf_file.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// tab_string is needed to be able switch between spaces "    " and tabs '\t'
char * tab_string = "    ";



inline int max(int a, int b){
    return a > b ? a : b;
}

#define KW_FUNCT_ARRAY_SIZE (3)
#define CONFIG_FILE "formatting.conf"

// struture to store text keyword text  space,tab,newline, function to execute  before/after printig the keyword
typedef struct t_kw_settings {
    int nl_before;
    int tab_before;
    int space_before;

    int nl_after;
    int tab_after;
    int space_after;

    int print_original_text;
    int print_case;
    char * text;

    int (*funct_before[KW_FUNCT_ARRAY_SIZE])();
    int (*funct_after [KW_FUNCT_ARRAY_SIZE])();

} t_kw_settings;



void debug_kw_settings(t_kw_settings s){
    extern FILE * yyout;
    fprintf(yyout,"\nspace_before %d , tab_before %d , nl_before %d , space_after %d , tab_after %d , nl_after %d\n , text %s "
           ,s.space_before,s.tab_before,s.nl_before,s.space_after,s.tab_after,s.nl_after, s.text);
    //printf("after %X %X %X\n", s.funct_after[0],s.funct_after[1],s.funct_after[2]);//debug string
    //printf("before %X %X %X\n", s.funct_before[0],s.funct_before[1],s.funct_before[2]);//debug string
}



int debug_p();// TODO : make separate .c and .h files



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

static void print_spacing(FILE * yyout, t_kw_settings s){
/* Prints all spacing, where 'spacing' means new lines, tabs and spaces

 * Spacing is printed in the following order:
 *   new lines
 *   tabs for current level of indentation (depends on global var)
 *   tabs specific for the keyword
 *   spaces

 * There are 2 sets of spacings printed - 'before' and 'after' the keyword.
 * 'After' part is printed at the start of next call to this funtion, because there is dependency of adjacent word spacings
 * (e.g. to avoid trailing tabs/spaces on the line)
 * Additionaly it lets to extract all spacing printing into this function - separating it from keyword printing
*/
    static int prev_nl=0, prev_tab=0, prev_space=0; // keep track of 'after' spacing from previous call
    int i=0, prev_spaces_printed=0, prev_tabs_printed=0;

    if(s.text[0]!='\0')
    {
        // Print spacing
        // .. last (key)word's 'after' part
        print_nlines(yyout, prev_nl);
        if(!s.nl_before){ // avoid trailing tabs/spaces 
            if( prev_nl > 0 ) print_tabs(yyout, currindent);
            print_tabs(yyout, prev_tab);
            print_spaces(yyout, prev_space);
            prev_tabs_printed=prev_tab;
            prev_spaces_printed=prev_space; // FIXME-prevspacesprinted: can lead to spaces preceeding tab char
        }

        // .. current (key)word's before part
        print_nlines(yyout, s.nl_before - prev_nl);
        if(s.nl_before > 0) print_tabs(yyout, currindent); // tabs - for general indentation
 
        //    print_X(,before-prev_printed) -> cnt(total X printed) = max(before, prev_printed)
        print_tabs(yyout, s.tab_before - prev_tabs_printed); // FIXME-prevspacesprinted: can lead to spaces preceeding tab char
        print_spaces(yyout, s.space_before - prev_spaces_printed);

        // Save settings for next function call - overwrite
        prev_nl    = s.nl_after;
        prev_tab   = s.tab_after;
        prev_space = s.space_after;
    }
    else
    {   // Save settings for next function call - not overwrite, but combine
        prev_nl    += s.nl_after;
        prev_tab   += s.tab_after;
        prev_space = max(s.space_after, prev_space);
    }
}


#define MAX_KEYWORD_SIZE (50)
enum{CASE_none,CASE_lower,CASE_UPPER,CASE_Initcap};
char* stocase(char* s_text, int s_case){
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

    print_spacing(yyout, s); // print spacing before keyword

    fprintf(yyout,"%s",stocase( s.print_original_text ? yytext : s.text , s.print_case)); // 1st deside what text to use (original or degault), then handle its case
    
    // call keyword specific functions. After fprintf
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_after[i] != NULL ; i++)
        s.funct_after[i]();
}



void echo_print(FILE * yyout, char * txt){
    int i=0, space_cnt=0, nl_cnt=0, length, nbr;

    t_kw_settings s;
    s.nl_before=s.tab_before=s.space_before=s.nl_after=s.tab_after=s.space_after=0;

    //count blank characters at the end of the text
    length = strlen(txt);
    for(i=length-1; txt[i]==' '  && i>=0; i--) space_cnt++;
    for(          ; txt[i]=='\n' && i>=0; i--) nl_cnt++; // 'i=..' omited to continue from where last loop has finished

    // Prepare text for print (i is used with value set by last loop)
    nbr=i+1;
    s.text = (char*) malloc((nbr+1)*sizeof(char));
    strncpy(s.text, txt, nbr);
    s.text[nbr]='\0';

    // Spacing
    s.nl_after = nl_cnt;
    s.space_after = space_cnt;
    print_spacing(yyout, s);

    // Print
    fprintf(yyout,"%s",s.text);
    free(s.text);
}



#define T_KW_SETTINGS_MACRO( NAME , ... ) \
    t_kw_settings NAME ;
#include "t_kw_settings_list.def"
#undef T_KW_SETTINGS_MACRO



void set_case(int keyword_case){
    #define T_KW_SETTINGS_MACRO( NAME , ... ) \
        NAME.print_case = keyword_case;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}


void set_text_original(int ind_original){
    #define T_KW_SETTINGS_MACRO( NAME , ... ) \
        NAME.print_original_text = ind_original;
    #include "t_kw_settings_list.def"
    #undef T_KW_SETTINGS_MACRO
}

void init_all_settings(){
    #define T_KW_SETTINGS_MACRO( NAME,nlb,tb,sb,nla,ta,sa,TEXT , fb1,fb2,fb3,fa1,fa2,fa3) \
        NAME.nl_before    = nlb;    \
        NAME.tab_before   = tb;     \
        NAME.space_before = sb;     \
                                    \
        NAME.nl_after     = nla;    \
        NAME.tab_after    = ta;     \
        NAME.space_after  = sa;     \
        NAME.print_original_text = 0; \
        NAME.print_case   = CASE_UPPER; \
        NAME.text         = TEXT;   \
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

int setting_value(char * setting_name, int * setting_values)
{
    #define T_KW_SETTINGS_MACRO( NAME, ... )    \
    if( strcmp(#NAME,setting_name) == 0 ){      \
        NAME.nl_before    = setting_values[0];  \
        NAME.tab_before   = setting_values[1];  \
        NAME.space_before = setting_values[2];  \
        NAME.nl_after     = setting_values[3];  \
        NAME.tab_after    = setting_values[4];  \
        NAME.space_after  = setting_values[5];  \
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
