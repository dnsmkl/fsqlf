#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"
#include "global_variables.h"

#define BEGIN_STATE(NEWSTATE) debug_stchange(NEWSTATE); BEGIN (NEWSTATE);
#define PUSH_STATE(NEWSTATE)  push_stack(YY_START); /*printf("\nPUSH");*/ BEGIN_STATE(NEWSTATE);
#define POP_STATE(); /*printf("\nPOP");*/ BEGIN_STATE(peek_stack()); pop_stack();

// this is variable to be able switch between spaces "    " and tabs '\t'
char * tab_string = "    ";

int new_line() {
    int i=0;
    printf("\n");
    for(i=0;i<currindent;i++)
        printf("%s",tab_string);
}



int sp_b(t_kw_settings s){
// sp_b - spacing before
    int i=0;

    for(i=0;i<s.nl_before;i++)
        printf("\n");

    if(s.nl_before>0)
        for(i=0;i<currindent;i++)
            printf("%s",tab_string);

    for(i=0;i<s.tab_before;i++)
        printf("%s",tab_string);
    for(i=0;i<s.space_before;i++)
        printf(" ");

}



int sp_a(t_kw_settings s){
// sp_a - spacing after
    int i=0;

    for(i=0;i<s.nl_after;i++)
        printf("\n");

    if(s.nl_after>0)
        for(i=0;i<currindent;i++)
            printf("%s",tab_string);

    for(i=0;i<s.tab_after;i++)
        printf("%s",tab_string);
    for(i=0;i<s.space_after;i++)
        printf(" ");
}

void kw_print(t_kw_settings s){
    
    if (s.function_before != NULL) s.function_before();
    sp_b(s);
    printf("%s",s.text);
    sp_a(s);
    if (s.function_after != NULL) s.function_after();
}



int p_level(){
    return left_p - right_p - subselect_level;
}



void print_inc_LEFTP() {left_p++ ; kw_print(kw_left_p);   }
void print_inc_RIGHTP(){right_p++; kw_print(kw_right_p);  }

void print_LEFTP_begin_SUB(){new_line();  debug_p();  kw_print(kw_left_p);  begin_SUB(); }

int begin_SUB(){left_p++; subselect_level++; currindent++;}
int end_SUB()  {subselect_level--; currindent--;}



#endif










