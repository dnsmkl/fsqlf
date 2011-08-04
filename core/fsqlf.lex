/*
Thanks to http://vsbabu.org/software/lsqlb.html
Vattekkat Satheesh Babu, on Dec 30, 1998
Helped to learn about flex a bit
*/

%top{ 
    /* This code goes at the "top" of the generated file. */
#include <stdio.h>
#include "global_variables.h"
#include "print_keywords.h"
#include "settings.h"
}


%{

//redefinition of macros will probably go here if needed

//#define YY_USER_ACTION fprintf(yyout,"\n rule (%d) - ",yy_act);
#define DMATCH(name) fprintf(yyout,"%20s is rule (%d) : ", name , yy_act);

%}


DIGIT   [0-9]
NUMBER  {DIGIT}+([.]{DIGIT}+)?
ID      [A-Za-z_][A-Za-z0-9_]*
SPACE   [ \t\n]
NONSPACE {NUMBER}|{STRING}|{DBOBJECT}
DBOBJECT    ({ID}[.]){0,2}{ID}


LEFTP   [(]
RIGHTP  [)]

SELECT  (?i:select|sel)
AS      (?i:as)
FROM    (?i:from)

IJOIN   (?i:(inner{SPACE}+)?JOIN)
LJOIN   (?i:left{SPACE}+(OUTER{SPACE}+)?JOIN)
RJOIN   (?i:right{SPACE}+(OUTER{SPACE}+)?JOIN)
FJOIN   (?i:full{SPACE}+(OUTER{SPACE}+)?JOIN)
CJOIN   (?i:cross{SPACE}+JOIN{SPACE}+)
ANYJOIN ({LJOIN}|{RJOIN}|{FJOIN}|{CJOIN})

ON      (?i:on)
WHERE   (?i:where)
SAMPLE  (?i:sample)
AND     (?i:and)
EXISTS  (?i:exists)
IN      (?i:in)

COMMA [,]

COMMENT_ONE_LINE [-]{2,}[^\n]*[\n]
COMMENT_ML_START [/][*]+
COMMENT_ML_PART1 [^*]+
COMMENT_ML_PART2 [*]+[^/]
COMMENT_ML_END   [*]+[/]

STRING (['][^']*['])+


%option noyywrap

%s stSELECT stFROM stWHERE stON stEXISTS stLEFTP stJOIN stIN stCOMMA stINLIST
%x stCOMMENTML stSTRING

%%
                /* SELECT ... FROM */
<INITIAL>{SELECT}           {BEGIN_STATE(stSELECT); kw_print(kw_select); };
<stSELECT,stCOMMA>{COMMA}   {BEGIN_STATE(stCOMMA); kw_print(kw_comma); };
<stSELECT,stCOMMA>{LEFTP}   {PUSH_STATE(stLEFTP ); kw_print(kw_left_p);  };
<stLEFTP>{LEFTP}            {PUSH_STATE(stLEFTP ); debug_match("{LEFTP}");kw_print(kw_left_p);  };
<stLEFTP>{COMMA}            {ECHO; };
<stLEFTP>{FROM}             {debug_match("{FROM}" ); kw_print(kw_from_2);};
<stLEFTP>{RIGHTP}           {POP_STATE(); kw_print(kw_right_p); };
<stSELECT,stCOMMA>{FROM}    {BEGIN_STATE(stFROM  );  kw_print(kw_from); };
<stLEFTP,stSELECT>{AS}      {debug_match("{AS}"  );kw_print(kw_as);};

                /* FROM ... JOIN ... ON ... WHERE */
<stON,stFROM>{IJOIN} { BEGIN_STATE(stJOIN)  ;kw_print(kw_inner_join); };
<stON,stFROM>{LJOIN} { BEGIN_STATE(stJOIN)  ;kw_print(kw_left_join); };
<stON,stFROM>{RJOIN} { BEGIN_STATE(stJOIN)  ;kw_print(kw_right_join); };
<stON,stFROM>{FJOIN} { BEGIN_STATE(stJOIN)  ;kw_print(kw_full_join); };
<stON,stFROM>{CJOIN} { BEGIN_STATE(stJOIN)  ;kw_print(kw_cross_join); };

<stJOIN>{ON}    {BEGIN_STATE(stON);   kw_print(kw_on); };
<stON>{AND}     {debug_match("{AND}");kw_print(kw_and);};

<stFROM,stJOIN,stON>{WHERE} {BEGIN_STATE(stWHERE ); kw_print(kw_where); };

<stWHERE>{AND}      {debug_match("{AND}"); kw_print(kw_and);};
<stWHERE>{EXISTS}   {PUSH_STATE(stEXISTS); kw_print(kw_exists); };
<stEXISTS>{LEFTP}   {BEGIN_STATE(INITIAL ); kw_print(kw_left_p_sub); };

<stWHERE>{IN}    { PUSH_STATE(stIN); kw_print(kw_in);};

<stIN>{LEFTP}    { ; }; // at this point its not clear if its subquery or constant list expression, '(' will be printed later
<stIN>{SELECT}   { kw_print(kw_left_p_sub); BEGIN_STATE(stSELECT); kw_print(kw_select); };
<stIN>{NONSPACE} { BEGIN_STATE(stINLIST);kw_print(kw_left_p); ECHO;};







{COMMENT_ML_START}     {PUSH_STATE(stCOMMENTML);printf("\n");ECHO;};
<stCOMMENTML>{COMMENT_ML_PART1}     {debug_match("COMMENT_ML_PART1") ; ECHO;};
<stCOMMENTML>{COMMENT_ML_PART2}     {debug_match("COMMENT_ML_PART2") ; ECHO;};
<stCOMMENTML>{COMMENT_ML_END}       {POP_STATE();ECHO;printf("\n");};

{COMMENT_ONE_LINE}     {ECHO;};

{STRING}               {ECHO;white_space_cnt=0;};


{LEFTP}     { kw_print(kw_left_p); };
{RIGHTP}    { 
                POP_STATE();
                if(subselect_level && p_level()-1 <= 0){
                    kw_print(kw_right_p_sub);
                } else {
                    debug_match("<wtf-leftp>");
                    kw_print(kw_right_p);
                }

            };
	
{SPACE}+     if(white_space_cnt==0){
                fprintf(yyout," ");
                white_space_cnt=1;
              }

";"          fprintf(yyout,"\n;");
{DBOBJECT}   {ECHO;white_space_cnt=0;}
{NUMBER}     {ECHO;white_space_cnt=0;}

<*>.         {debug_match("<*>.");ECHO;white_space_cnt=0;};



<<EOF>> {
            fprintf(yyout,"\n");
            switch(YY_START){
                case stCOMMENTML: fprintf(yyout,"--unterminated comment \n"); break;
                case stSTRING: fprintf(yyout,"--unterminated  string\n"); break;
                default: ;
            }
            return 0 ;
        }

%%





#include "debuging.h"




int main(int argc, char **argv)
{
    switch(argc) // set yyin and yyout based on command line options
    {
    case 1:
	yyin  = stdin;
	yyout = stdout;
	break;
    case 2:
	yyin  = fopen(argv[1],"r");
	yyout = stdout;
	break;
    case 3:
	yyin  = fopen(argv[1],"r");
	yyout = fopen(argv[2],"w+");
	break;
    default:
	fprintf(stderr,"Usage:\n", argv[0] );
	fprintf(stderr,"\t%s \t\t\t# input from stding , output to stdout\n", argv[0] );
	fprintf(stderr,"\t%s input_file\t\t# input from a file, output to stdout\n", argv[0] );
	fprintf(stderr,"\t%s input_file output_file\t# input and output from files\n", argv[0] );
	return 2;
    }

    if( yyin == NULL || yyout == NULL ){
	fprintf(stderr,"%s: unable to open %s or output\n", argv[0] , argv[1]);
	return 3;
    }

    while (yylex () != 0) ;

    return 0;
}

