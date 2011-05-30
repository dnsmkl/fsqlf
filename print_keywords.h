#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"
#include "global_variables.h"

#define BEGIN_STATE(NEWSTATE) debug_stchange(NEWSTATE); BEGIN (NEWSTATE);
#define PUSH_STATE(NEWSTATE)  push_stack(YY_START); /*printf("\nPUSH");*/ BEGIN_STATE(NEWSTATE);
#define POP_STATE(); /*printf("\nPOP");*/ BEGIN_STATE(peek_stack()); pop_stack();

// tab_string is needed to be able switch between spaces "    " and tabs '\t'
char * tab_string = "    ";


// YY_USER_INIT is lex macro executed before initialising parser
#define YY_USER_INIT init_all_settings();



// struture to store text keyword text  space,tab,newline, function to execute  before/after printig the keyword
typedef struct t_kw_settings {
    int nl_before;
    int tab_before;
    int space_before;

    int nl_after;
    int tab_after;
    int space_after;

    char * text;

    int (*function_before)();
    int (*function_after)();
} t_kw_settings;



void kw_set(t_kw_settings* kw_set
    , int nl_before, int tab_before, int space_before
    , int nl_after , int tab_after , int space_after
    , char * text )
{
    kw_set->nl_before    = nl_before;
    kw_set->tab_before   = tab_before;
    kw_set->space_before = space_before;

    kw_set->nl_after     = nl_after;
    kw_set->tab_after    = tab_after;
    kw_set->space_after  = space_after;
    kw_set->text         = text;

    kw_set->function_before = NULL;
    kw_set->function_after  = NULL;

}



void kw_add_funct(t_kw_settings* kw_set,  int (*f1)(), int (*f2)() )
{
    kw_set->function_before = f1;
    kw_set->function_after  = f2;
}



void debug_kw_settings(t_kw_settings s){
    printf("\nspace_before %d , tab_before %d , nl_before %d , space_after %d , tab_after %d , nl_after %d\n , text %s "
           ,s.space_before,s.tab_before,s.nl_before,s.space_after,s.tab_after,s.nl_after, s.text);
}





t_kw_settings kw_comma, kw_select, kw_inner_join, kw_left_join, kw_right_join, kw_full_join, kw_cross_join, kw_from, kw_on, kw_where, kw_and, kw_exists, kw_in, kw_from_2, kw_as;

t_kw_settings kw_left_p, kw_right_p;

int debug_p();

void init_all_settings(){
   kw_set(&kw_comma      ,1,0,0,0,0,0,",");
   kw_set(&kw_select     ,1,0,0,1,0,2,"SelecT");
   kw_set(&kw_inner_join ,1,0,0,0,0,1,"JoiN");
   kw_set(&kw_left_join  ,1,0,0,0,0,1,"LefT JoiN");
   kw_set(&kw_right_join ,1,0,0,0,0,1,"RighT JoiN");
   kw_set(&kw_full_join  ,1,0,0,0,0,1,"FulL JoiN");
   kw_set(&kw_cross_join ,1,0,0,0,0,1,"CrosS JoiN");
   kw_set(&kw_from       ,1,0,0,0,0,1,"FroM");
   kw_set(&kw_on         ,1,0,0,0,0,1,"oN");
   kw_set(&kw_where      ,1,0,0,0,0,1,"WherE");
   kw_set(&kw_and        ,1,0,0,0,0,1,"AnD");
   kw_set(&kw_exists     ,0,0,0,0,0,1,"exists");
   kw_set(&kw_in         ,0,0,0,0,0,1,"in");
   kw_set(&kw_from_2     ,0,0,1,0,0,1,"from");
   kw_set(&kw_as         ,0,0,1,0,0,1,"as");
   kw_set(&kw_left_p     ,0,0,0,0,0,0,"(");      kw_add_funct( &kw_left_p , &debug_p, NULL );
   kw_set(&kw_right_p    ,0,0,0,0,0,0,")");      kw_add_funct( &kw_right_p, &debug_p, NULL );
}



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

void print_LEFTP_begin_SUB(){new_line();  kw_print(kw_left_p);  begin_SUB(); }

int begin_SUB(){left_p++; subselect_level++; currindent++;}
int end_SUB()  {subselect_level--; currindent--;}



#endif










