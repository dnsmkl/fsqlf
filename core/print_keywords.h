#ifndef print_keywords_h
#define print_keywords_h


#include <string.h>     // strlen
#include <ctype.h>      // tolower, toupper
#include <stdio.h>      // fprintf, fputs
#include "kw/kw.h"      // All kw settings as global variables


void handle_kw(FILE *yyout, char *yytext, struct kw_conf *s);
void handle_text(FILE *yyout, char *txt);


#endif

