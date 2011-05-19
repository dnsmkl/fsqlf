#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"
#include "global_variables.h"

#define BEGIN_STATE(NEWSTATE) debug_stchange(NEWSTATE); BEGIN (NEWSTATE);
#define PUSH_STATE(NEWSTATE)  push_stack(YY_START); /*printf("\nPUSH");*/ BEGIN_STATE(NEWSTATE);
#define POP_STATE(); /*printf("\nPOP");*/ BEGIN_STATE(peek_stack()); pop_stack();


int printm() {int i=0;    for(i=0;i<currindent;i++)        printf("    ");}
int new_line() {    int i=0;    printf("\n");    for(i=0;i<currindent;i++)        printf("    ");}





int p_level(){
    return left_p - right_p - subselect_level;
}




void print_SELECT(){new_line();printf("SELECT");   new_line(); printf("  ");}
void print_stFROM()  {new_line();printf( "FROM " );}
void print_WHERE() {new_line();printf( "WHERE " );}
void print_IJOIN() {new_line();printf( "JOIN " );       }
void print_LJOIN() {new_line();printf( "LEFT JOIN " );  }
void print_RJOIN() {new_line();printf( "RIGHT JOIN " ); }
void print_FJOIN() {new_line();printf( "FULL JOIN " );  }
void print_CJOIN() {new_line();printf( "CROSS JOIN " ); }
void print_ON()    {new_line();printf( " ON " ); }
void print_AND()   {new_line();printf("AND ")  ; }


void print_COMMA_stSELECT(){  new_line();printf( ", ");}
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










