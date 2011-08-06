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

    kw_set->funct_before[0] = NULL;
    kw_set->funct_after[0]  = NULL;

}



void kw_set_funct_before(t_kw_settings* kw_set, int nr, int (*f)() )
{
    extern FILE * yyout;
    if( nr<0 || nr>=KW_FUNCT_ARRAY_SIZE) {
        fprintf(yyout,"\n -- error: kw_set_funct_before - bounds.   n = %d\n", nr);
        return;
    }

    kw_set->funct_before[nr] = f;
}

void kw_set_funct_after(t_kw_settings* kw_set, int nr, int (*f)() )
{
    extern FILE * yyout;
    if( nr<0 || nr>=KW_FUNCT_ARRAY_SIZE) {
        fprintf(yyout,"\n -- error: kw_set_funct_after - bounds.   n = %d\n", nr);
        return;
    }

    kw_set->funct_after[nr] = f;
}



void debug_kw_settings(t_kw_settings s){
    extern FILE * yyout;
    fprintf(yyout,"\nspace_before %d , tab_before %d , nl_before %d , space_after %d , tab_after %d , nl_after %d\n , text %s "
           ,s.space_before,s.tab_before,s.nl_before,s.space_after,s.tab_after,s.nl_after, s.text);
    //printf("after %X %X %X\n", s.funct_after[0],s.funct_after[1],s.funct_after[2]);//debug string
    //printf("before %X %X %X\n", s.funct_before[0],s.funct_before[1],s.funct_before[2]);//debug string
}





t_kw_settings kw_comma, kw_select, kw_inner_join, kw_left_join, kw_right_join, kw_full_join, kw_cross_join, kw_from, kw_on, kw_where, kw_and, kw_exists, kw_in, kw_from_2, kw_as;

t_kw_settings kw_left_p, kw_right_p, kw_left_p_sub, kw_right_p_sub, kw_or;

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
   kw_set(&kw_comma      ,1,0,0,0,0,1,",");
   kw_set(&kw_select     ,1,0,0,1,0,2,"SELECT");
   kw_set(&kw_inner_join ,1,0,0,0,0,1,"JOIN");
   kw_set(&kw_left_join  ,1,0,0,0,0,1,"LEFT JOIN");
   kw_set(&kw_right_join ,1,0,0,0,0,1,"RIGHT JOIN");
   kw_set(&kw_full_join  ,1,0,0,0,0,1,"FULL JOIN");
   kw_set(&kw_cross_join ,1,0,0,0,0,1,"CROSS JOIN");
   kw_set(&kw_from       ,1,0,0,0,0,1,"FROM");
   kw_set(&kw_on         ,1,0,1,0,0,1,"ON");
   kw_set(&kw_where      ,1,0,0,0,0,1,"WHERE");
   kw_set(&kw_and        ,1,0,0,0,0,1,"AND");
   kw_set(&kw_or         ,1,0,0,0,0,1,"OR");
   kw_set(&kw_exists     ,0,0,0,0,0,1,"exists");
   kw_set(&kw_in         ,0,0,0,0,0,1,"in");
   kw_set(&kw_from_2     ,0,0,1,0,0,1,"from");
   kw_set(&kw_as         ,0,0,1,0,0,1,"as");
   kw_set(&kw_left_p     ,0,0,0,0,0,0,"(");
   kw_set(&kw_right_p    ,0,0,0,0,0,0,")");
   kw_set(&kw_left_p_sub ,1,0,0,0,0,0,"(");
   kw_set(&kw_right_p_sub,1,0,0,1,0,0,")");

   kw_set_funct_before( &kw_left_p     ,0, &debug_p   );
   kw_set_funct_before( &kw_left_p     ,1, &inc_LEFTP );

   kw_set_funct_before( &kw_right_p    ,0, &debug_p   );
   kw_set_funct_before( &kw_right_p    ,1, &inc_RIGHTP);

   kw_set_funct_before( &kw_left_p_sub ,0, &debug_p  );
   kw_set_funct_before( &kw_left_p_sub ,1, &inc_LEFTP);
   kw_set_funct_after( &kw_left_p_sub ,0, &begin_SUB  );

   kw_set_funct_before( &kw_right_p_sub,0, &debug_p   );
   kw_set_funct_before( &kw_right_p_sub,1, &inc_RIGHTP);
   kw_set_funct_before( &kw_right_p_sub,2, &end_SUB   );
}


#endif

