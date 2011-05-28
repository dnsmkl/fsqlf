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



int sp_b(t_spacing_settings s){
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



int sp_a(t_spacing_settings s){
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





int p_level(){
    return left_p - right_p - subselect_level;
}




void print_SELECT(){new_line();printf("SELECT");   new_line(); printf("  ");}

void print_COMMA_stSELECT(){
     sp_b(sp_comma);
     printf( ",");
     sp_a(sp_comma);
}

void print_stFROM()  {new_line();printf( "FROM " );}
void print_WHERE() {new_line();printf( "WHERE " );}
void print_IJOIN() {new_line();printf( "JOIN " );       }
void print_LJOIN() {new_line();printf( "LEFT JOIN " );  }
void print_RJOIN() {new_line();printf( "RIGHT JOIN " ); }
void print_FJOIN() {new_line();printf( "FULL JOIN " );  }
void print_CJOIN() {new_line();printf( "CROSS JOIN " ); }
void print_ON()    {new_line();printf( " ON " ); }
void print_AND()   {new_line();printf("AND ")  ; }



void print_EXISTS(){ printf("EXISTS");}
void print_AS()    { printf(" as ");}
void print_FROM()  { printf( " from " );}

void print_LEFTP() { debug_p();printf("(") ; }
void print_RIGHTP(){ debug_p();printf(")") ; }

void print_inc_LEFTP() {left_p++ ; printf("(") ; debug_p(); }
void print_inc_RIGHTP(){right_p++; printf(")") ; debug_p(); }

void begin_SUB(){left_p++; subselect_level++; currindent++;}
void end_SUB()  {subselect_level--; currindent--;}
void print_LEFTP_begin_SUB(){new_line();  debug_p();  printf("(") ;  begin_SUB(); }

print_IN()  { printf("in ");}

#endif










