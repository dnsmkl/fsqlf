#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"
#include "global_variables.h"
#include <stdio.h>

#define BEGIN_STATE(NEWSTATE) debug_stchange(NEWSTATE); BEGIN (NEWSTATE);
#define PUSH_STATE(NEWSTATE)  push_stack(YY_START); /*printf("\nPUSH");*/ BEGIN_STATE(NEWSTATE);
#define POP_STATE(); /*printf("\nPOP");*/ BEGIN_STATE(peek_stack()); pop_stack();

// tab_string is needed to be able switch between spaces "    " and tabs '\t'
char * tab_string = "    ";


// YY_USER_INIT is lex macro executed before initialising parser
#define YY_USER_INIT init_all_settings();


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




#define T_KW_SETTINGS_MACRO( NAME , ... ) \
    t_kw_settings NAME ;
#include "t_kw_settings_list.def"
#undef T_KW_SETTINGS_MACRO


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

    for(i=0;i<s.nl_before;i++)
        fprintf(yyout,"\n");

    if(s.nl_before>0)
        for(i=0;i<currindent;i++)
            fprintf(yyout,"%s",tab_string);

    for(i=0;i<s.tab_before;i++)
        fprintf(yyout,"%s",tab_string);
    for(i=0;i<s.space_before;i++)
        fprintf(yyout," ");

}



int sp_a(t_kw_settings s){
// sp_a - spacing after
    extern FILE * yyout;
    int i=0;

    for(i=0;i<s.nl_after;i++)
        fprintf(yyout,"\n");

    if(s.nl_after>0)
        for(i=0;i<currindent;i++)
            fprintf(yyout,"%s",tab_string);

    for(i=0;i<s.tab_after;i++)
        fprintf(yyout,"%s",tab_string);
    for(i=0;i<s.space_after;i++)
        fprintf(yyout," ");

    white_space_cnt=s.nl_after+s.tab_after+s.space_after;
}

void kw_print(t_kw_settings s){
    extern FILE * yyout;
    int i=0;
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_before[i] != NULL ; i++)
        s.funct_before[i]();
    //if (s.text[0] == '(') fprintf(yyout,"\t\t ** for - before :  i is %d", i);//debug line

    sp_b(s);
    fprintf(yyout,"%s",s.text);
    sp_a(s);
    for(i=0; i < KW_FUNCT_ARRAY_SIZE && s.funct_after[i] != NULL ; i++)
        s.funct_after[i]();
    //if (s.text[0] == '(') fprintf(yyout,"\t\t ** for - after :  i is %d", i);//debug line
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

