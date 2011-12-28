#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"
#include "global_variables.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// tab_string is needed to be able switch between spaces "    " and tabs '\t'
char * tab_string = "    ";



inline int max(int a, int b){
    return a > b ? a : b;
}

#define KW_FUNCT_ARRAY_SIZE (3)

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



static int sp_b(FILE * yyout, t_kw_settings s, int no_nl, int no_space ){
// sp_b - spacing before
    int i=0, minus_sp=0, minus_tb=0, minus_nl=0;
    static int prev_nl=0, prev_tab=0, prev_space=0; // settings saved from previously printed (key)word for spacing after it

    if(s.text[0]!='\0')
    {
        // spacing from last (key)word
        for(i=0; i < prev_nl; i++) fprintf(yyout,"\n");
        if(!s.nl_before){
            if( prev_nl > 0 ) for(i=0; i<currindent; i++)   fprintf(yyout,"%s",tab_string); // tabs - for indentation
            for(i=0; i < prev_tab   ; i++) fprintf(yyout,"%s",tab_string);
            for(i=0; i < prev_space ; i++) fprintf(yyout," ");
        }

        // spacing before (key)word
        for(i=0; i < s.nl_before - prev_nl ; i++) fprintf(yyout,"\n"); // new lines
        minus_tb=(s.nl_before?0:prev_tab);
        minus_sp=(s.nl_before?0:prev_space);

        if(s.nl_before > 0 ) for(i=0; i<currindent; i++) fprintf(yyout,"%s",tab_string); // tabs - for general indentation
        for(i=0; i < s.tab_before - minus_tb; i++)       fprintf(yyout,"%s",tab_string); // tabs
        for(i=0; i < s.space_before - minus_sp; i++)  fprintf(yyout," "); // spaces

        // save settings for next function call
        prev_nl    = s.nl_after;
        prev_tab   = s.tab_after;
        prev_space = s.space_after;
    }
    else
    {   // save settings, but not just overwrite
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

    sp_b(yyout, s, 0, 0); // print spacing before keyword

    fprintf(yyout,"%s",stocase( s.print_original_text ? yytext : s.text , s.print_case)); // 1st deside what text to use (original or degault), then handle its case
    
    // call keyword specific functions. After fprintf
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_after[i] != NULL ; i++)
        s.funct_after[i]();
}



void echo_print(FILE * yyout, char * txt){
    int i=0, space_cnt=0, nl_cnt=0, length, nbr;
    char *tmp_txt;

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
    sp_b(yyout, s, 0, 0);

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


int read_configs()
{
    FILE * config_file;
    char line[BUFFER_SIZE+1] , setting_name[BUFFER_SIZE+1];
    int setting_values[VALUE_NUMBER];
    char * chr_ptr1,chr_ptr2;
    int i;


    if( ! (config_file=fopen("formatting.conf","r")) ){
        printf("failed to open config file");
        exit(1);
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
