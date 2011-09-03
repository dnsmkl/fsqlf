#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"
#include "global_variables.h"
#include <stdio.h>
#include <string.h>



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



int new_line() {
    extern FILE * yyout;
    int i=0;
    fprintf(yyout,"\n");
    for(i=0;i<currindent;i++)
        fprintf(yyout,"%s",tab_string);
}



int sp_b(t_kw_settings s){
// sp_b - spacing before
    extern FILE * yyout;
    int i=0;
    static int prev_nl=0, prev_tab=0, prev_space=0; // settings saved from previously printed keyword for spacing after it

    // spacing from last (key)word
    if(prev_nl) white_space_cnt = 0;
    for(i=0; i < prev_nl - new_line_cnt      ; i++) fprintf(yyout,"\n");
    if(!s.nl_before){
        if( prev_nl > 0 ) for(i=0; i<currindent; i++)   fprintf(yyout,"%s",tab_string);// tabs - for indentation
        for(i=0; i < prev_tab                    ; i++) fprintf(yyout,"%s",tab_string);
        for(i=0; i < prev_space - white_space_cnt; i++) fprintf(yyout," ");
    }

    // spacing before (key)word
    if(s.nl_before) prev_tab = prev_space = white_space_cnt = 0;
    for(i=0; i < s.nl_before - prev_nl - new_line_cnt; i++)           fprintf(yyout,"\n"); // new lines
    if(s.nl_before > 0 ) for(i=0; i<currindent; i++)                  fprintf(yyout,"%s",tab_string); // tabs - for general indentation
    for(i=0; i < s.tab_before - prev_tab; i++)                        fprintf(yyout,"%s",tab_string); // tabs
    for(i=0; i < s.space_before - prev_space - white_space_cnt; i++)  fprintf(yyout," "); // spaces

    // save settings for next call
    prev_nl=s.nl_after;
    prev_tab=s.tab_after;
    prev_space=s.space_after; 
}



void kw_print(t_kw_settings s){
    extern FILE * yyout;
    int i=0;
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_before[i] != NULL ; i++)
        s.funct_before[i]();

    sp_b(s);
    fprintf(yyout,"%s",s.text);
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_after[i] != NULL ; i++)
        s.funct_after[i]();

    white_space_cnt = 0;
    new_line_cnt = 0;
}



void echo_print(char * txt){
    extern FILE * yyout;
    int i=0;
    
    t_kw_settings s;
    s.nl_before=s.tab_before=s.space_before=s.nl_after=s.tab_after=s.space_after=0;
   
    //count spaces and new lines at the end of the string
    for(i=strlen(txt)-1; txt[i]==' '; i--) white_space_cnt++;
    for(               ; txt[i]=='\n'; i--) new_line_cnt++; // 'i=..' omited to continue from where last loop has finished

    sp_b(s);
    fprintf(yyout,"%s",txt);

    white_space_cnt = 0;
    new_line_cnt = 0;
}



#define T_KW_SETTINGS_MACRO( NAME , ... ) \
    t_kw_settings NAME ;
#include "t_kw_settings_list.def"
#undef T_KW_SETTINGS_MACRO



void init_all_settings(){
    #define T_KW_SETTINGS_MACRO( NAME,nlb,tb,sb,nla,ta,sa,TEXT , fb1,fb2,fb3,fa1,fa2,fa3) \
        NAME.nl_before    = nlb;    \
        NAME.tab_before   = tb;     \
        NAME.space_before = sb;     \
                                    \
        NAME.nl_after     = nla;    \
        NAME.tab_after    = ta;     \
        NAME.space_after  = sa;     \
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


#endif
