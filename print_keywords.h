#ifndef print_keywords_h
#define print_keywords_h

#include "settings.h"

#define BEGIN_STATE(NEWSTATE) debug_stchange2(NEWSTATE); BEGIN (NEWSTATE);


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
void print_LEFTP() { printf("(") ;}
void print_RIGHTP(){ printf(")") ;}
void print_AS()    { printf(" as ");}
void print_FROM()  { printf( " from " );}

void begin_SUB(){left_p++; subselect_level++; currindent++;}
void print_LEFTP_begin_SUB(){new_line();    printf("(") ; begin_SUB();}

#endif










