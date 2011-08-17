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
char * state_to_char(int);
//#define YY_USER_ACTION fprintf(yyout,"\n %10s - rule (%d) - line(%d) " ,state_to_char(YY_START),yy_act, __LINE__);
#define DMATCH(name) fprintf(yyout,"%20s is rule (%d) : ", name , yy_act);



#define BEGIN_STATE(NEWSTATE) debug_stchange(NEWSTATE); BEGIN (NEWSTATE);
#define PUSH_STATE(NEWSTATE)  push_stack(YY_START); /*printf("\nPUSH");*/ BEGIN_STATE(NEWSTATE);
#define POP_STATE(); /*printf("\nPOP");*/ BEGIN_STATE(peek_stack()); pop_stack();

// YY_USER_INIT is lex macro executed before initialising parser
#define YY_USER_INIT init_all_settings();

%}


DIGIT   [0-9]
NUMBER  {DIGIT}+([.]{DIGIT}+)?
ID      [A-Za-z_][A-Za-z0-9_]*
SPACE   [ \t\n]
DBOBJECT    ({ID}[.]){0,2}{ID}


LEFTP   [(]
RIGHTP  [)]

UNION     (?i:UNION)
UNION_ALL (?i:UNION[ ]ALL)
INTERSECT (?i:intersect)
EXCEPT    (?i:except)

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
OR      (?i:or)
EXISTS  (?i:exists)
IN      (?i:in)
COMPARISON (=|<>|<=|>=|<|>)

COMMA [,]

COMMENT_ONE_LINE [-]{2,}[^\n]*[\n]
COMMENT_ML_START [/][*]+
COMMENT_ML_PART1 [^*]+
COMMENT_ML_PART2 [*]+[^/]
COMMENT_ML_END   [*]+[/]

STRING (['][^']*['])+


%option noyywrap

%s stSELECT stFROM stWHERE stON stEXISTS stLEFTP stJOIN stIN stCOMMA stINLIST stFROM_LEFTP stP_SUB
%x stCOMMENTML stSTRING

%%

                /* SET operations */
{UNION}      {BEGIN_STATE(INITIAL);kw_print(kw_union)    ;};
{UNION_ALL}  {BEGIN_STATE(INITIAL);kw_print(kw_union_all);};
{INTERSECT}  {BEGIN_STATE(INITIAL);kw_print(kw_intersect);};
{EXCEPT}     {BEGIN_STATE(INITIAL);kw_print(kw_except);};

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

                /* parantheses in FROM clause: posible subselect , grouped joins */
<stFROM_LEFTP>{IJOIN}  { BEGIN_STATE(stJOIN)  ; kw_print(kw_left_p); kw_print(kw_inner_join); };
<stFROM_LEFTP>{LJOIN}  { BEGIN_STATE(stJOIN)  ; kw_print(kw_left_p); kw_print(kw_left_join); };
<stFROM_LEFTP>{RJOIN}  { BEGIN_STATE(stJOIN)  ; kw_print(kw_left_p); kw_print(kw_right_join); };
<stFROM_LEFTP>{FJOIN}  { BEGIN_STATE(stJOIN)  ; kw_print(kw_left_p); kw_print(kw_full_join); };
<stFROM_LEFTP>{CJOIN}  { BEGIN_STATE(stJOIN)  ; kw_print(kw_left_p); kw_print(kw_cross_join); };
<stFROM_LEFTP>{DBOBJECT} { BEGIN_STATE(peek_stack()); kw_print(kw_left_p); ECHO; white_space_cnt=0; }

<stJOIN>{ON}    {BEGIN_STATE(stON);   kw_print(kw_on); };



                /* WHERE ... (also join conditions) */
<stFROM,stJOIN,stON>{WHERE} {BEGIN_STATE(stWHERE ); kw_print(kw_where); };
<stWHERE,stON,stJOIN>{AND}   { debug_match("{AND}"); kw_print(kw_and);};
<stWHERE,stON,stJOIN>{OR}    { debug_match("{OR}"); kw_print(kw_or);};

<stWHERE>{EXISTS}   {kw_print(kw_exists); };



<stP_SUB>{LEFTP}                      { BEGIN_STATE(peek_stack()); kw_print(kw_left_p    ); PUSH_STATE(stP_SUB);  };
{LEFTP}                               { PUSH_STATE(stP_SUB); };
<stP_SUB>{SELECT}                     { BEGIN_STATE(stSELECT);     kw_print(kw_left_p_sub); kw_print(kw_select);};
<stP_SUB>{NUMBER}|{STRING}|{DBOBJECT} { BEGIN_STATE(peek_stack()); kw_print(kw_left_p    ); ECHO; white_space_cnt=0; }
<stP_SUB>{COMMENT_ML_START}           { BEGIN_STATE(peek_stack()); kw_print(kw_left_p    ); PUSH_STATE(stCOMMENTML); printf("\n"); ECHO; white_space_cnt=0;};
<stP_SUB>{COMMENT_ONE_LINE}           { BEGIN_STATE(peek_stack()); kw_print(kw_left_p    ); ECHO;};
<stP_SUB>.                            { BEGIN_STATE(peek_stack()); kw_print(kw_left_p    ); ECHO;};

{RIGHTP}    {
                POP_STATE();
                if(subselect_level && p_level()-1 <= 0){
                    kw_print(kw_right_p_sub);
                } else {
                    debug_match("<wtf-leftp>");
                    kw_print(kw_right_p);
                }

            };




{COMMENT_ML_START}     {PUSH_STATE(stCOMMENTML);printf("\n");ECHO;};
<stCOMMENTML>{COMMENT_ML_PART1}     {debug_match("COMMENT_ML_PART1") ; ECHO;};
<stCOMMENTML>{COMMENT_ML_PART2}     {debug_match("COMMENT_ML_PART2") ; ECHO;};
<stCOMMENTML>{COMMENT_ML_END}       {POP_STATE();ECHO;printf("\n");};

{COMMENT_ONE_LINE}     {ECHO;};


{STRING}     {ECHO;white_space_cnt=0;};
{SPACE}+     {
                if(white_space_cnt==0) fprintf(yyout," ");
                white_space_cnt=1;
             }
{DBOBJECT}   {ECHO;white_space_cnt=0;}
{NUMBER}     {ECHO;white_space_cnt=0;}
<*>.         {debug_match("<*>.");ECHO;white_space_cnt=0;};
";"          fprintf(yyout,"\n;");


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
	fprintf(stderr,"\t%s                         # read from stdin, write to stdout\n", argv[0] );
	fprintf(stderr,"\t%s input_file              # read from file, write to stdout\n", argv[0] );
	fprintf(stderr,"\t%s input_file output_file  # use files for reading and writing\n", argv[0] );
	return 2;
    }

    if( yyin == NULL || yyout == NULL ){
	fprintf(stderr,"%s: unable to open %s or output\n", argv[0] , argv[1]);
	return 3;
    }

    while (yylex () != 0) ;

    return 0;
}

